#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "datawindow.h"
#include "sourcewindow.h"
#include "registerwindow.h"
#include "floatwindow.h"
#include "terminalwindow.h"
#include "consolewindow.h"
#include "projectwindow.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QDockWidget;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);

private slots:
    void increaseFont();
    void decreaseFont();
    void displayHelp();
    void setDataDockVisible(bool);
    void setRegisterDockVisible(bool);
    void setFloatDockVisible(bool);
    void setProjectDockVisible(bool);
    void setTerminalDockVisible(bool);
    void setConsoleDockVisible(bool);
    void quit();


private:
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void helpAction ( QMenu *menu, QString s, QString file );
    void keyPressEvent ( QKeyEvent *event );
    void addToggle ( QMenu *menu, QString text, QObject *object, const char *slot );

    DataWindow *data;
    SourceWindow *source;
    RegisterWindow *registerWindow;
    FloatWindow *floatWindow;
    ProjectWindow *project;
    TerminalWindow *terminal;
    ConsoleWindow *console;
    Settings *settings;

    QDockWidget *dataDock;
    QDockWidget *registerDock;
    QDockWidget *floatDock;
    QDockWidget *projectDock;
    QDockWidget *terminalDock;
    QDockWidget *consoleDock;

    void setFontSize();
    int fontSize;

    QMenu *fileMenu;
    QMenu *templateMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *moveMenu;
    QMenu *fontMenu;
    QMenu *helpMenu;
};

#endif
