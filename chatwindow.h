#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(const QString &doctorType, const QString &doctorName, QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void sendMessage();

private:
    QString doctorType;
    QString doctorName;
    QTextEdit *chatBox;
    QLineEdit *inputField;
    QPushButton *sendButton;
    QNetworkAccessManager *networkManager;

    void setupUI();
    void applyStyle();
    void sendInitialGreeting();
};

#endif // CHATWINDOW_H
