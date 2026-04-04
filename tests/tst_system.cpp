#include <QtTest/QtTest>
#include "system.h"

class TestSystem : public QObject
{
    Q_OBJECT

private slots:
    void defaultValues();
    void setCarLocked_changesValue();
    void setCarLocked_noSignalWhenUnchanged();
    void setOutdoorTemp_changesValue();
    void setOutdoorTemp_noSignalWhenUnchanged();
    void setUserName_changesValue();
    void setUserName_noSignalWhenUnchanged();
    void currentTime_isNotEmpty();
};

void TestSystem::defaultValues()
{
    System s;
    QCOMPARE(s.carLocked(), true);
    QCOMPARE(s.outdoorTemp(), 32);
    QCOMPARE(s.userName(), QString("Artaxerxes I"));
    QVERIFY(!s.currentTime().isEmpty());
}

void TestSystem::setCarLocked_changesValue()
{
    System s;
    QSignalSpy spy(&s, &System::carLockedChanged);
    s.setCarLocked(false);
    QCOMPARE(s.carLocked(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

void TestSystem::setCarLocked_noSignalWhenUnchanged()
{
    System s; // default: locked = true
    QSignalSpy spy(&s, &System::carLockedChanged);
    s.setCarLocked(true); // same value
    QCOMPARE(spy.count(), 0);
}

void TestSystem::setOutdoorTemp_changesValue()
{
    System s;
    QSignalSpy spy(&s, &System::outdoorTempChanged);
    s.setOutdoorTemp(25);
    QCOMPARE(s.outdoorTemp(), 25);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 25);
}

void TestSystem::setOutdoorTemp_noSignalWhenUnchanged()
{
    System s; // default: 32
    QSignalSpy spy(&s, &System::outdoorTempChanged);
    s.setOutdoorTemp(32);
    QCOMPARE(spy.count(), 0);
}

void TestSystem::setUserName_changesValue()
{
    System s;
    QSignalSpy spy(&s, &System::userNameChanged);
    s.setUserName("Darius");
    QCOMPARE(s.userName(), QString("Darius"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("Darius"));
}

void TestSystem::setUserName_noSignalWhenUnchanged()
{
    System s;
    QSignalSpy spy(&s, &System::userNameChanged);
    s.setUserName("Artaxerxes I");
    QCOMPARE(spy.count(), 0);
}

void TestSystem::currentTime_isNotEmpty()
{
    System s;
    QVERIFY(!s.currentTime().isEmpty());
    // Time format is "hh:mm ap" — verify it looks right
    QVERIFY(s.currentTime().contains(QRegularExpression("\\d{2}:\\d{2} (am|pm)")));
}

QTEST_MAIN(TestSystem)
#include "tst_system.moc"
