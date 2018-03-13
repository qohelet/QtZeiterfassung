#include "webradiosettings.h"

#include "zeiterfassungsettings.h"

WebRadioSettings::WebRadioSettings(ZeiterfassungSettings &settings) :
    m_settings(settings)
{
}

QStringList WebRadioSettings::urls() const
{
    return m_settings.value(QStringLiteral("WebRadioPlugin/urls"), QStringList {
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
                            }).toStringList();
}

void WebRadioSettings::setUrls(const QStringList &urls)
{
    m_settings.setValue(QStringLiteral("WebRadioPlugin/urls"), urls);
}
