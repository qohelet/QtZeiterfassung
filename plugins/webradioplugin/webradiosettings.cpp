#include "webradiosettings.h"

#include "zeiterfassungsettings.h"

const QString WebRadioSettings::m_urls("WebRadioPlugin/urls");
const QString WebRadioSettings::m_lastUrl("WebRadioPlugin/lastUrl");
const QString WebRadioSettings::m_volume("WebRadioPlugin/volume");
const QStringList WebRadioSettings::m_defaultUrls {
    QStringLiteral("http://stream.drumandbass.fm:9002"),
    QStringLiteral("http://stream.trap.fm:6002"),
    QStringLiteral("http://stream.dubbase.fm:7002"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/hb.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/tb.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/tt.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/ht.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/trb.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/ct.mp3"),
    QStringLiteral("http://lw1.mp3.tb-group.fm/clt.mp3"),
    QStringLiteral("https://live.helsinki.at:8088/live160.ogg")
};
const int WebRadioSettings::m_defaultVolume(100);

WebRadioSettings::WebRadioSettings(ZeiterfassungSettings &settings, QObject *parent) :
    QObject(parent),
    m_settings(settings)
{
}

QStringList WebRadioSettings::urls() const
{
    return m_settings.value(m_urls, m_defaultUrls).toStringList();
}

void WebRadioSettings::setUrls(const QStringList &urls)
{
    if(this->urls() != urls)
    {
        m_settings.setValue(m_urls, urls);
        Q_EMIT urlsChanged(urls);
    }
}

QString WebRadioSettings::lastUrl() const
{
    return m_settings.value(m_lastUrl).toString();
}

void WebRadioSettings::setLastUrl(const QString &lastUrl)
{
    if(this->lastUrl() != lastUrl)
    {
        m_settings.setValue(m_lastUrl, lastUrl);
        Q_EMIT lastUrlChanged(lastUrl);
    }
}

int WebRadioSettings::volume() const
{
    return m_settings.value(m_volume, m_defaultVolume).toInt();
}

void WebRadioSettings::setVolume(int volume)
{
    if(this->volume() != volume)
    {
        m_settings.setValue(m_volume, volume);
        Q_EMIT volumeChanged(volume);
    }
}
