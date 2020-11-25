#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MarkdownHandler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    fontSize = new QComboBox();
    fontSize->addItem(tr("Normal"), 0);
    fontSize->addItem(tr("Title 1"), 1);
    fontSize->addItem(tr("Title 2"), 2);
    fontSize->addItem(tr("Title 3"), 3);
    fontSize->addItem(tr("Title 4"), 4);
    fontSize->addItem(tr("Title 5"), 5);
    fontSize->addItem(tr("Title 6"), 6);
    ui->toolBar->addWidget(fontSize);
    setActions();
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(openTab()));
}

// to-do : contextual menu
// to-do : deactivate actions if no tab
// to-do : override Os keyboard shortcut

MainWindow::~MainWindow()
{
    delete ui;
}

void    MainWindow::setActions()
{
    connect(ui->actionBold, SIGNAL(triggered()), this, SLOT(formatBold()));
    connect(ui->actionItalic, SIGNAL(triggered()), this, SLOT(formatItalic()));
    connect(ui->actionLink, SIGNAL(triggered()), this, SLOT(formatLink()));
    connect(ui->actionCode, SIGNAL(triggered()), this, SLOT(formatCodeSnippet()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(fontSize, SIGNAL(activated(int)), this, SLOT(formatTitle(int)));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->actionRule, SIGNAL(triggered()), this, SLOT(printRule()));
    connect(ui->actionList, SIGNAL(triggered()), this, SLOT(formatOList()));
    connect(ui->actionUList, SIGNAL(triggered()), this, SLOT(formatUList()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionQuote, SIGNAL(triggered()), this, SLOT(formatQuote()));
    connect(ui->actionStrike, SIGNAL(triggered()), this, SLOT(formatStrikethrough()));
}

void MainWindow::onTextChanged()
{
    QTextEdit *currentTab = getCurrentTab();
    QString md = currentTab->toPlainText();
    ui->textBrowser->setMarkdown(md);
}

void    MainWindow::openTab()
{
   openTab("Nouveau Document");
}

void    MainWindow::openTab(QString title)
{
    QTextEdit       *newTextEdit;
    newTextEdit = new TextEdit(this);
    ui->tabWidget->addTab(newTextEdit, title);
    connect(newTextEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    ui->tabWidget->setCurrentWidget(newTextEdit);
}

QTextEdit   *MainWindow::getCurrentTab()
{
    return qobject_cast<QTextEdit *>(ui->tabWidget->currentWidget());
}


void    MainWindow::formatBold()
{
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::wrapText(activeTab, "**");
    /*
    QTextCursor cursor = activeTab->textCursor();
    QString     prefix = "**";

    if (QRegularExpression("(\\*\\*|__)(.*?)\\1").match(cursor.selectedText()).hasMatch())
        cursor.insertText(cursor.selectedText().replace(QRegularExpression("(\\*\\*|__)"), ""));
    else
        activeTab->insertPlainText(prefix + cursor.selectedText() + prefix);
*/
}

void    MainWindow::formatItalic()
{
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::wrapText(activeTab, "*");
    /*
    QTextCursor cursor = activeTab->textCursor();
    QString     prefix = "*";


    if (QRegularExpression("(\\*\\*|__)(.*?)\\1").match(cursor.selectedText()).hasMatch())
        activeTab->insertPlainText(prefix + cursor.selectedText() + prefix);
    else if (QRegularExpression("\\*").match(cursor.selectedText()).hasMatch())
        cursor.insertText(cursor.selectedText().replace(QRegularExpression("\\*"), ""));
    else
        activeTab->insertPlainText(prefix + cursor.selectedText() + prefix);*/
    //to-do : unset italic if already set + improve detection (regex)
}

void    MainWindow::formatLink()
{
    QTextEdit   *activeTab = getCurrentTab();
    QTextCursor cursor = activeTab->textCursor();
    activeTab->insertPlainText("["+ cursor.selectedText() + "]()");
    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
    activeTab->setTextCursor(cursor);
}

void    MainWindow::formatTitle(int t)
{
    QTextEdit   *activeTab = getCurrentTab();
    QString prefix;

    for (int i = 1; i <= t; i++)
    {
        prefix += "#";
    }
    if (t != 0)
        prefix += " ";
    QTextCursor cursor = activeTab->textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.insertText(cursor.selectedText().replace(QRegularExpression("(^(?:#+ )|^ *)"), prefix));
}

void    MainWindow::formatCodeSnippet()
{
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::wrapParagraph(activeTab, "```\r", "\r```");
    /*
    QString     prefix;
    QTextCursor cursor = activeTab->textCursor();
    int selectedLines = 0;

    if (!cursor.selectedText().isEmpty())
    {
        QString str = cursor.selection().toPlainText();
        selectedLines = str.count("\n")+1;
    }
    prefix = (selectedLines > 1) ? "```" : "`";
    activeTab->insertPlainText(prefix + cursor.selectedText() + prefix);
    */
    //to-do : unset code if already set
}

void    MainWindow::save()
{
    QTextEdit   *activeTab = getCurrentTab();

    if (activeTab == NULL)
        return;
    QString defaultPath;
    defaultPath += "/tmp/";
    defaultPath += ui->tabWidget->currentWidget()->windowTitle();
    defaultPath += ".md";
    QString fileName = QFileDialog::getSaveFileName(activeTab, tr("Enregistrer le document"), defaultPath, "");
    if (fileName.isEmpty())
            return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(activeTab, tr("Impossible d'ouvrir le fichier"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        out << activeTab->toPlainText();
     }
}

void    MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Ouvrir un fichier"), "",
            tr("All Files (*.md)"));
    if (fileName.isEmpty())
            return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);
        openTab(fileName);
        QTextEdit   *activeTab = getCurrentTab();
        activeTab->append(in.readAll());
    }
}

void    MainWindow::copy()
{
    QTextEdit  *activeTab = getCurrentTab();
    activeTab->copy();
}

void    MainWindow::cut()
{
    QTextEdit  *activeTab = getCurrentTab();
    activeTab->cut();
}

void    MainWindow::paste()
{
    QTextEdit  *activeTab = getCurrentTab();
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    QString tmp = originalText;
    QTextCursor cursor = activeTab->textCursor();

    if (QRegularExpression("^(https?|ftp|file)://[-a-zA-Z0-9+&@#/%?=~_|!:,.;]*[-a-zA-Z0-9+&@#/%=~_|]").match(originalText).hasMatch())
    {
        if (cursor.hasSelection())
            clipboard->setText("[" + cursor.selection().toPlainText() + "](" + originalText + ")");
        else
            clipboard->setText("[" + originalText + "](" + originalText + ")");
    }
    activeTab->paste();
    clipboard->setText(tmp);
}

void MainWindow::printRule()
{
    QTextEdit   *activeTab = getCurrentTab();
    QTextCursor cursor = activeTab->textCursor();
    cursor.insertText("\n----\n");
}

void    MainWindow::close()
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void    MainWindow::formatUList()
{
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::prependEachLine(activeTab, "* ");
}

void    MainWindow::formatOList()
{
    //to-do : unordered and ordered lists
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::prependEachLine(activeTab, "1. ");

    //to-do : unset list if already set
    //to-do : transform unordered list to ordered list and vice-versa
    //to-do : set empty list item if cursor has no selection


    // "^(?:\d+\.|[*+-]) .*(?:\r?\n(?!(?:\d+\.|[*+-]) ).*)*/gm"
}

void    MainWindow::formatQuote()
{
    QTextEdit   *activeTab = getCurrentTab();

    if (MarkdownHandler::isAlreadyPrefixed(activeTab, QRegularExpression("^ ?>+ ?")))
        MarkdownHandler::unPrependEachLine(activeTab, QRegularExpression("^ ?>+ ?"));
    else
        MarkdownHandler::prependEachLine(activeTab, "> ");
}

void    MainWindow::formatStrikethrough()
{
    QTextEdit   *activeTab = getCurrentTab();

    MarkdownHandler::wrapText(activeTab, "~~");
}
