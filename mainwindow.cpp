#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("VitaSync");
    this->setWindowState(Qt::WindowMaximized);

    setupUI();
    applyStyle();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("🌟 Welcome to VitaSync 🌟", this);
    QFont titleFont("Segoe UI", 32, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #333333;");

    generalDoctorBtn = new QPushButton("🩺 Dr.Driva", this);
    psychiatricDoctorBtn = new QPushButton("🧠 Mood Mentor", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(30);
    layout->addWidget(generalDoctorBtn);
    layout->addSpacing(20);
    layout->addWidget(psychiatricDoctorBtn);
    layout->addStretch();

    layout->setAlignment(Qt::AlignCenter);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(generalDoctorBtn, &QPushButton::clicked, this, &MainWindow::openGeneralDoctorChat);
    connect(psychiatricDoctorBtn, &QPushButton::clicked, this, &MainWindow::openPsychiatricDoctorChat);
}

void MainWindow::applyStyle() {
    QFont buttonFont("Segoe UI", 20, QFont::Bold);
    generalDoctorBtn->setFont(buttonFont);
    psychiatricDoctorBtn->setFont(buttonFont);

    QString styleTemplate = R"(
        QPushButton {
            background-color: %1;
            color: white;
            border: none;
            border-radius: 20px;
            padding: 20px 40px;
            font-weight: bold;
            letter-spacing: 1px;
        }
        QPushButton:hover {
            background-color: white;
            color: %1;
            border: 2px solid %1;
        }
    )";

    generalDoctorBtn->setStyleSheet(styleTemplate.arg("#FF6F61")); // Coral pink
    psychiatricDoctorBtn->setStyleSheet(styleTemplate.arg("#6A5ACD")); // Purple

    // Drop shadows for buttons
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setBlurRadius(15);
    shadow1->setOffset(0, 5);
    shadow1->setColor(QColor(0, 0, 0, 120));
    generalDoctorBtn->setGraphicsEffect(shadow1);

    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect(this);
    shadow2->setBlurRadius(15);
    shadow2->setOffset(0, 5);
    shadow2->setColor(QColor(0, 0, 0, 120));
    psychiatricDoctorBtn->setGraphicsEffect(shadow2);

    // Gradient background
    centralWidget->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
                         stop: 0 #e0f7fa, stop: 1 #ffe0f7);
        }
    )");
}

void MainWindow::openGeneralDoctorChat() {
    ChatWindow *chat = new ChatWindow("General Doctor");
    chat->show();
}

void MainWindow::openPsychiatricDoctorChat() {
    ChatWindow *chat = new ChatWindow("Psychiatric Doctor");
    chat->show();
}
