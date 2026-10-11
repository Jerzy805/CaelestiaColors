#pragma once

#include <QMainWindow>

class ClickableFrame;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    ClickableFrame* m_selectedCard = nullptr;
};