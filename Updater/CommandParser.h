#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

class CommandParser
{
public:
    CommandParser();
    ~CommandParser() = default;

    // Sollte nicht kopiert werden
    CommandParser(const CommandParser& ) = delete;
    CommandParser& operator = (const CommandParser& ) = delete;

    // Sollte nicht verschoben werden
    CommandParser(CommandParser&& ) = delete;
    CommandParser& operator = (CommandParser&& ) = delete;

    void Process(QApplication& app);

    bool Qiet() const { return m_parser.isSet(m_qiet); }
    bool OnlySearch() const { return m_parser.isSet(m_search); }
    bool StartUpdate() const { return m_parser.isSet(m_update); }
    bool UpdateUrl() const { return m_parser.isSet(m_url); }
    QString UpdateUrlString() const { return m_parser.value(m_url); }

private:
    QCommandLineParser m_parser;

    QCommandLineOption m_qiet; // Zeige keine GUI
    QCommandLineOption m_search; // Suche nur nach Updates
    QCommandLineOption m_update; // Starte Update
    QCommandLineOption m_url; // URL zum Updateserver
};

#endif // COMMANDPARSER_H
