#include "mediacontroller.h"
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QMediaMetaData>
#include <QDebug>

MediaController::MediaController(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_playlist(new QMediaPlaylist(this))
    , m_positionTimer(new QTimer(this))
    , m_currentTitle("No Track")
    , m_currentArtist("Unknown Artist")
    , m_currentTime(0)
    , m_totalTime(0)
    , m_volume(50)
    , m_shuffle(false)
    , m_repeat(false)
{
    // Supported audio formats
    m_supportedFormats << "*.mp3" << "*.mp4" << "*.wav" << "*.ogg" 
                       << "*.m4a" << "*.aac" << "*.flac" << "*.wma";

    // Setup media player
    m_player->setPlaylist(m_playlist);
    m_player->setVolume(m_volume);

    // Connect signals
    connect(m_player, &QMediaPlayer::stateChanged, this, &MediaController::handleStateChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MediaController::handlePositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MediaController::handleDurationChanged);
    connect(m_player, &QMediaPlayer::currentMediaChanged, this, &MediaController::handleCurrentMediaChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MediaController::handleMediaStatusChanged);
    connect(m_player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            this, &MediaController::handleError);

    // Position update timer
    connect(m_positionTimer, &QTimer::timeout, this, &MediaController::updateCurrentTime);
    m_positionTimer->setInterval(1000); // Update every second

    // Setup playlist
    m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);

    // Auto-load music directory
    loadMusicDirectory();
}

MediaController::~MediaController()
{
    if (m_player) {
        m_player->stop();
    }
}

// Property getters
bool MediaController::isPlaying() const
{
    return m_player->state() == QMediaPlayer::PlayingState;
}

QString MediaController::currentTitle() const
{
    return m_currentTitle;
}

QString MediaController::currentArtist() const
{
    return m_currentArtist;
}

qint64 MediaController::currentTime() const
{
    return m_currentTime;
}

qint64 MediaController::totalTime() const
{
    return m_totalTime;
}

int MediaController::volume() const
{
    return m_volume;
}

QStringList MediaController::playlist() const
{
    return m_playlistFiles;
}

int MediaController::currentIndex() const
{
    return m_playlist->currentIndex();
}

bool MediaController::shuffle() const
{
    return m_shuffle;
}

bool MediaController::repeat() const
{
    return m_repeat;
}

// Media control slots
void MediaController::play()
{
    if (m_playlist->mediaCount() > 0) {
        m_player->play();
        m_positionTimer->start();
    }
}

void MediaController::pause()
{
    m_player->pause();
    m_positionTimer->stop();
}

void MediaController::stop()
{
    m_player->stop();
    m_positionTimer->stop();
}

void MediaController::togglePlayPause()
{
    if (isPlaying()) {
        pause();
    } else {
        play();
    }
}

void MediaController::next()
{
    if (m_shuffle) {
        // Random next track
        int randomIndex = qrand() % m_playlist->mediaCount();
        m_playlist->setCurrentIndex(randomIndex);
    } else {
        m_playlist->next();
    }
}

void MediaController::previous()
{
    m_playlist->previous();
}

// Playlist management
void MediaController::loadMusicDirectory(const QString &path)
{
    QString musicPath = path.isEmpty() ? "music" : path;
    
    // Try relative path first, then absolute
    QDir musicDir(musicPath);
    if (!musicDir.exists()) {
        musicPath = QCoreApplication::applicationDirPath() + "/music";
        musicDir.setPath(musicPath);
    }
    
    if (!musicDir.exists()) {
        qWarning() << "Music directory not found:" << musicPath;
        emit mediaError("Music directory not found: " + musicPath);
        return;
    }

    clearPlaylist();
    
    QStringList audioFiles = getSupportedAudioFiles(musicDir);
    
    qDebug() << "Found" << audioFiles.size() << "audio files in" << musicPath;
    
    for (const QString &filePath : audioFiles) {
        addFile(filePath);
    }
    
    if (m_playlist->mediaCount() > 0) {
        m_playlist->setCurrentIndex(0);
        qDebug() << "Loaded" << m_playlist->mediaCount() << "tracks";
    }
}

void MediaController::addFile(const QString &filePath)
{
    QUrl url = QUrl::fromLocalFile(filePath);
    m_playlist->addMedia(QMediaContent(url));
    
    QFileInfo fileInfo(filePath);
    m_playlistFiles.append(fileInfo.baseName());
    
    emit playlistChanged();
}

void MediaController::removeFile(int index)
{
    if (index >= 0 && index < m_playlist->mediaCount()) {
        m_playlist->removeMedia(index);
        m_playlistFiles.removeAt(index);
        emit playlistChanged();
    }
}

void MediaController::clearPlaylist()
{
    m_playlist->clear();
    m_playlistFiles.clear();
    emit playlistChanged();
}

void MediaController::playTrack(int index)
{
    if (index >= 0 && index < m_playlist->mediaCount()) {
        m_playlist->setCurrentIndex(index);
        play();
    }
}

// Settings
void MediaController::setVolume(int volume)
{
    int clampedVolume = qBound(0, volume, 100);
    if (m_volume != clampedVolume) {
        m_volume = clampedVolume;
        m_player->setVolume(m_volume);
        emit volumeChanged(m_volume);
    }
}

void MediaController::setShuffle(bool shuffle)
{
    if (m_shuffle != shuffle) {
        m_shuffle = shuffle;
        if (shuffle) {
            m_playlist->setPlaybackMode(QMediaPlaylist::Random);
        } else if (m_repeat) {
            m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        } else {
            m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        }
        emit shuffleChanged(m_shuffle);
    }
}

void MediaController::setRepeat(bool repeat)
{
    if (m_repeat != repeat) {
        m_repeat = repeat;
        if (repeat) {
            m_playlist->setPlaybackMode(m_shuffle ? QMediaPlaylist::Random : QMediaPlaylist::Loop);
        } else {
            m_playlist->setPlaybackMode(m_shuffle ? QMediaPlaylist::Random : QMediaPlaylist::Sequential);
        }
        emit repeatChanged(m_repeat);
    }
}

void MediaController::seek(qint64 position)
{
    m_player->setPosition(position);
}

// Private slots
void MediaController::handleStateChanged(QMediaPlayer::State state)
{
    emit isPlayingChanged(state == QMediaPlayer::PlayingState);
    
    if (state == QMediaPlayer::PlayingState) {
        m_positionTimer->start();
    } else {
        m_positionTimer->stop();
    }
}

void MediaController::handlePositionChanged(qint64 position)
{
    if (m_currentTime != position) {
        m_currentTime = position;
        emit currentTimeChanged(m_currentTime);
    }
}

void MediaController::handleDurationChanged(qint64 duration)
{
    if (m_totalTime != duration) {
        m_totalTime = duration;
        emit totalTimeChanged(m_totalTime);
    }
}

void MediaController::handleCurrentMediaChanged(const QMediaContent &content)
{
    if (content.isNull()) {
        m_currentTitle = "No Track";
        m_currentArtist = "Unknown Artist";
    } else {
        QString filePath = content.canonicalUrl().toLocalFile();
        extractMetadata(filePath);
    }
    
    emit currentTitleChanged(m_currentTitle);
    emit currentArtistChanged(m_currentArtist);
    emit currentIndexChanged(m_playlist->currentIndex());
}

void MediaController::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::InvalidMedia) {
        emit mediaError("Invalid media file");
    } else if (status == QMediaPlayer::EndOfMedia) {
        // Handle end of track
        if (!m_repeat && !m_shuffle && m_playlist->currentIndex() == m_playlist->mediaCount() - 1) {
            // End of playlist
            stop();
        }
    }
}

void MediaController::handleError(QMediaPlayer::Error error)
{
    QString errorString;
    switch (error) {
    case QMediaPlayer::ResourceError:
        errorString = "Resource error - media file not found or corrupt";
        break;
    case QMediaPlayer::FormatError:
        errorString = "Format error - unsupported media format";
        break;
    case QMediaPlayer::NetworkError:
        errorString = "Network error";
        break;
    case QMediaPlayer::AccessDeniedError:
        errorString = "Access denied - insufficient permissions";
        break;
    case QMediaPlayer::ServiceMissingError:
        errorString = "Service missing - required media service not available";
        break;
    default:
        errorString = "Unknown media error";
        break;
    }
    
    qWarning() << "Media player error:" << errorString;
    emit mediaError(errorString);
}

void MediaController::updateCurrentTime()
{
    // This ensures regular updates even if positionChanged isn't emitted frequently
    qint64 position = m_player->position();
    if (m_currentTime != position) {
        m_currentTime = position;
        emit currentTimeChanged(m_currentTime);
    }
}

// Private helper methods
void MediaController::extractMetadata(const QString &filePath)
{
    m_currentTitle = getFileTitle(filePath);
    m_currentArtist = getFileArtist(filePath);
    
    // Try to get metadata from QMediaPlayer if available
    if (m_player->isMetaDataAvailable()) {
        QString title = m_player->metaData(QMediaMetaData::Title).toString();
        QString artist = m_player->metaData(QMediaMetaData::Author).toString();
        
        if (!title.isEmpty()) m_currentTitle = title;
        if (!artist.isEmpty()) m_currentArtist = artist;
    }
}

QString MediaController::getFileTitle(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    
    // Simple parsing for common filename patterns
    if (baseName.contains(" - ")) {
        QStringList parts = baseName.split(" - ");
        if (parts.size() >= 2) {
            return parts[1].trimmed(); // Assume "Artist - Title" format
        }
    }
    
    return baseName;
}

QString MediaController::getFileArtist(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    
    // Simple parsing for common filename patterns
    if (baseName.contains(" - ")) {
        QStringList parts = baseName.split(" - ");
        if (parts.size() >= 2) {
            return parts[0].trimmed(); // Assume "Artist - Title" format
        }
    }
    
    return "Unknown Artist";
}

QStringList MediaController::getSupportedAudioFiles(const QDir &dir)
{
    QStringList files;
    
    QFileInfoList fileList = dir.entryInfoList(m_supportedFormats, QDir::Files | QDir::Readable, QDir::Name);
    
    for (const QFileInfo &fileInfo : fileList) {
        files.append(fileInfo.absoluteFilePath());
    }
    
    return files;
}