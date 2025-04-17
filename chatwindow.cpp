#include "chatwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QKeyEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

ChatWindow::ChatWindow(const QString &doctorType, const QString &doctorName, QWidget *parent)
    : QWidget(parent), doctorType(doctorType), doctorName(doctorName)
{
    setWindowTitle("Chat with " + doctorName);
    showMaximized();

    setupUI();
    applyStyle();
    sendInitialGreeting();
}

ChatWindow::~ChatWindow() {}

void ChatWindow::setupUI()
{
    chatBox = new QTextEdit();
    chatBox->setReadOnly(true);
    chatBox->setStyleSheet(R"(
        QTextEdit {
            font-size: 16px;
            background-color: #000000;
            color: #ffffff;
            border: 1px solid #444;
            padding: 10px;
        }
    )");

    inputField = new QLineEdit();
    inputField->setPlaceholderText("Type your message...");
    inputField->setMinimumHeight(40);
    inputField->setStyleSheet(R"(
        QLineEdit {
            font-size: 16px;
            padding: 8px;
            border: 1px solid #444;
            border-radius: 6px;
            background-color: #1e1e1e;
            color: #ffffff;
        }
    )");

    sendButton = new QPushButton("Send");
    sendButton->setFixedHeight(40);
    sendButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3498db;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #2471a3;
        }
    )");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatBox);
    layout->addLayout(inputLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);

    networkManager = new QNetworkAccessManager(this);
}

void ChatWindow::applyStyle()
{
    // Different styling based on doctor type
    if (doctorType == "General") {
        this->setStyleSheet(R"(
            QWidget {
                background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                    stop:0 #1e3c72, stop:1 #2a5298);
            }
        )");
    } else { // Psychiatric
        this->setStyleSheet(R"(
            QWidget {
                background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,
                    stop:0 #614385, stop:1 #516395);
            }
        )");
    }
}

void ChatWindow::sendInitialGreeting()
{
    QString greeting;
    if (doctorType == "General") {
        greeting = QString("Hello! I'm %1, your general health assistant. "
                           "I can help with medical questions, symptoms analysis, "
                           "and general health advice. How can I help you today?").arg(doctorName);
    } else { // Psychiatric
        greeting = QString("Hi there, I'm %1, your mental wellness guide. "
                           "I'm here to listen and help with any emotional or psychological concerns "
                           "you might have. What's on your mind?").arg(doctorName);
    }

    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    chatBox->append("<b>" + doctorName + " [" + time + "]:</b> " + greeting);
}

void ChatWindow::sendMessage()
{
    QString message = inputField->text().trimmed();
    if (message.isEmpty()) return;

    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    chatBox->append("<b>You [" + time + "]:</b> " + message);
    inputField->clear();

    // Prepare system prompt based on doctor type
    QString systemPrompt;
    if (doctorType == "General") {
        systemPrompt = QString("You are %1, a professional and kind general practitioner. "
                               "Provide accurate medical information in simple terms. "
                               "For serious symptoms, recommend seeing a doctor in person. "
                               "Be concise but thorough in your explanations.").arg(doctorName);
    } else { // Psychiatric
        systemPrompt = QString("You are %1, a compassionate mental health counselor. "
                               "Provide supportive, non-judgmental advice. "
                               "For serious mental health concerns, recommend professional help. "
                               "Use active listening techniques and ask thoughtful questions.").arg(doctorName);
    }

    // Prepare JSON payload
    QJsonArray messagesArray = {
        QJsonObject{
            {"role", "system"},
            {"content", systemPrompt}
        },
        QJsonObject{
            {"role", "user"},
            {"content", message}
        }
    };

    QJsonObject requestBody;
    requestBody["model"] = "meta-llama/llama-3.1-8b-instruct:free";
    requestBody["messages"] = messagesArray;

    QNetworkRequest request(QUrl("https://openrouter.ai/api/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer ");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(requestBody).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QString responseText = jsonDoc["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();

            QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
            chatBox->append("<b>" + doctorName + " [" + time + "]:</b> " + responseText);
        } else {
            chatBox->append("<b>Error:</b> " + reply->errorString());
        }
        reply->deleteLater();
    });
}
