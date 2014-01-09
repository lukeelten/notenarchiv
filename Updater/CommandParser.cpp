#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>
#include <QString>
#include <QStringList>

#include "CommandParser.h"

CommandParser::CommandParser() : m_parser(),
    m_qiet(QStringList() << "q" << "qiet", "Qiet mode. Shows no interface."),
    m_search(QStringList() << "s" << "search", "Searches for new updates."),
    m_update(QStringList() << "u" << "update", "Starts the update automatically."),
    m_url("url", "Set the Update-Url", "url"),
    m_version("local-version", "The local version of Notenarchiv", "local-version")
{
    m_parser.setApplicationDescription("Updater für das Programm Notenarchiv");
    m_parser.addHelpOption();
    m_parser.addVersionOption();

    m_parser.addOption(m_qiet);
    m_parser.addOption(m_search);
    m_parser.addOption(m_update);
    m_parser.addOption(m_url);
    m_parser.addOption(m_version);
}

void CommandParser::Process(QApplication &app) {
    m_parser.process(app);
}
