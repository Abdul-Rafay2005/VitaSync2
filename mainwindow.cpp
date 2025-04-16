#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QFrame>
#include <QPalette>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include "chatwindow.h"  // Assuming you have a ChatWindow class

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QPushButton *generalDoctorBtn;
    QPushButton *psychiatricDoctorBtn;
    QLabel *titleLabel;  // Add this line
    QFrame *separator;
    QPalette palette;
    QGraphicsDropShadowEffect *shadowEffect;

    void setupUI();
    void applyStyle();
    void applyButtonStyle(QPushButton *button, const QString &color);

private slots:
    void openGeneralDoctorChat();
    void openPsychiatricDoctorChat();
};

#endif // MAINWINDOW_H
