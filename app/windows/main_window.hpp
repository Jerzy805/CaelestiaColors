#pragma once

#include <QMainWindow>

#include <caelestia_colors/scheme_editor.hpp>
#include <optional>

class ClickableFrame;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    ClickableFrame* m_selectedCard = nullptr;
    std::optional<SchemeEditor> m_scheme_editor;
};