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

ChatWindow::ChatWindow(const QString &doctorType, QWidget *parent)
    : QWidget(parent), doctorType(doctorType)
{
    setWindowTitle("Chat with " + doctorType);
    showMaximized();

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

void ChatWindow::sendMessage()
{
    QString message = inputField->text().trimmed();
    if (message.isEmpty()) return;

    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    chatBox->append("<b>You [" + time + "]:</b> " + message);
    inputField->clear();

    // Prepare JSON payload
    QJsonArray messagesArray = {
        QJsonObject{
            {"role", "system"},
            {"content", "You are a helpful, kind and professional AI medical assistant. Only respond to health and therapy-related questions. Politely decline anything unrelated."}
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
    request.setRawHeader("Authorization", "Bearer our api key");
    // request.setRawHeader("HTTP-Referer", "<YOUR_SITE_URL>");
    // request.setRawHeader("X-Title", "<YOUR_SITE_NAME>");

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(requestBody).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
            QString responseText = jsonDoc["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();

            QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
            chatBox->append("<b>VitaSync [" + time + "]:</b> " + responseText);
        } else {
            chatBox->append("<b>Error:</b> " + reply->errorString());
        }
        reply->deleteLater();
    });
}
