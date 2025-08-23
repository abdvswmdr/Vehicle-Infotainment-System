#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QStringList>
#include <QDir>
#include <QTimer>

class MediaController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentTitleChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY currentArtistChanged)
    Q_PROPERTY(qint64 currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(qint64 totalTime READ totalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QStringList playlist READ playlist NOTIFY playlistChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

public:
    explicit MediaController(QObject *parent = nullptr);
    ~MediaController();

    // Property getters
    bool isPlaying() const;
    QString currentTitle() const;
    QString currentArtist() const;
    qint64 currentTime() const;
    qint64 totalTime() const;
    int volume() const;
    QStringList playlist() const;
    int currentIndex() const;
    bool shuffle() const;
    bool repeat() const;

public slots:
    // Media control
    void play();
    void pause();
    void stop();
    void togglePlayPause();
    void next();
    void previous();
    
    // Playlist management
    void loadMusicDirectory(const QString &path = "");
    void addFile(const QString &filePath);
    void removeFile(int index);
    void clearPlaylist();
    void playTrack(int index);
    
    // Settings
    void setVolume(int volume);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void seek(qint64 position);

signals:
    void isPlayingChanged(bool isPlaying);
    void currentTitleChanged(const QString &title);
    void currentArtistChanged(const QString &artist);
    void currentTimeChanged(qint64 currentTime);
    void totalTimeChanged(qint64 totalTime);
    void volumeChanged(int volume);
    void playlistChanged();
    void currentIndexChanged(int index);
    void shuffleChanged(bool shuffle);
    void repeatChanged(bool repeat);
    void mediaError(const QString &error);

private slots:
    void handleStateChanged(QMediaPlayer::State state);
    void handlePositionChanged(qint64 position);
    void handleDurationChanged(qint64 duration);
    void handleCurrentMediaChanged(const QMediaContent &content);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void handleError(QMediaPlayer::Error error);
    void updateCurrentTime();

private:
    void extractMetadata(const QString &filePath);
    QString getFileTitle(const QString &filePath);
    QString getFileArtist(const QString &filePath);
    QStringList getSupportedAudioFiles(const QDir &dir);
    
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    QTimer *m_positionTimer;
    
    // Current track info
    QString m_currentTitle;
    QString m_currentArtist;
    qint64 m_currentTime;
    qint64 m_totalTime;
    
    // Settings
    int m_volume;
    bool m_shuffle;
    bool m_repeat;
    
    // File list for playlist display
    QStringList m_playlistFiles;
    
    // Supported audio formats
    QStringList m_supportedFormats;
};

#endif // MEDIACONTROLLER_H