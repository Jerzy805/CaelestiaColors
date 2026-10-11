
#include "windows/main_window.hpp"
#include "widgets/clickable_frame.hpp"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QVector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("CaelestiaColours");
    resize(1280, 800);
    setMinimumSize(950, 600);

    auto* root = new QWidget(this);
    root->setObjectName("root");
    setCentralWidget(root);

    auto* rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(16);

    // Top bar
    auto* header = new QHBoxLayout;
    header->setSpacing(10);

    auto* title = new QLabel("CaelestiaColours"); // label title
    title->setObjectName("appTitle");

    auto* subtitle = new QLabel("SCHEME EDITOR");
    subtitle->setObjectName("subtitle");

    auto* titleGroup = new QVBoxLayout;
    titleGroup->setSpacing(2);
    titleGroup->addWidget(title);
    titleGroup->addWidget(subtitle);

    auto* newSchemeButton = new QPushButton("+  New scheme");
    newSchemeButton->setObjectName("primaryButton");

    header->addLayout(titleGroup);
    header->addStretch();
    header->addWidget(newSchemeButton);

    rootLayout->addLayout(header);

    // Main workspace
    auto* workspace = new QHBoxLayout;
    workspace->setSpacing(14);

    // Sidebar
    auto* sidebar = new QFrame;
    sidebar->setObjectName("panel");
    sidebar->setMinimumWidth(190);
    sidebar->setMaximumWidth(240);

    auto* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(14, 16, 14, 14);
    sidebarLayout->setSpacing(12);

    auto* schemesTitle = new QLabel("YOUR SCHEMES");
    schemesTitle->setObjectName("sectionTitle");

    auto* search = new QLabel("⌕   Search schemes...");
    search->setObjectName("searchPlaceholder");

    auto* schemeList = new QListWidget;
    schemeList->setObjectName("schemeList");
    schemeList->addItems({
        "Catppuccin",
        "Nord",
        "Gruvbox",
        "Tokyo Night"
    });
    schemeList->setCurrentRow(0);

    sidebarLayout->addWidget(schemesTitle);
    sidebarLayout->addWidget(search);
    sidebarLayout->addWidget(schemeList, 1);

    // Palette preview
    auto* editor = new QFrame;
    editor->setObjectName("panel");

    auto* editorLayout = new QVBoxLayout(editor);
    editorLayout->setContentsMargins(22, 20, 22, 22);
    editorLayout->setSpacing(18);

    auto* paletteTitle = new QLabel("Catppuccin");
    paletteTitle->setObjectName("panelTitle");

    auto* paletteDescription = new QLabel(
        "Your colours, organised in one place."
    );
    paletteDescription->setObjectName("mutedText");

    auto* paletteHeader = new QVBoxLayout;
    paletteHeader->setSpacing(5);
    paletteHeader->addWidget(paletteTitle);
    paletteHeader->addWidget(paletteDescription);

    editorLayout->addLayout(paletteHeader);

    auto* colors = new QGridLayout;
    colors->setSpacing(12);

    const QStringList colorNames = {
        "Rosewater", "Flamingo", "Pink", "Mauve",
        "Red", "Maroon", "Peach", "Yellow",
        "Green", "Teal", "Sky", "Sapphire",
        "Blue", "Lavender", "Text", "Base"
    };

    const QStringList colorValues = {
        "#f5e0dc", "#f2cdcd", "#f5c2e7", "#cba6f7",
        "#f38ba8", "#eba0ac", "#fab387", "#f9e2af",
        "#a6e3a1", "#94e2d5", "#89dceb", "#74c7ec",
        "#89b4fa", "#b4befe", "#cdd6f4", "#1e1e2e"
    };

    QVector<ClickableFrame*> colourCards;

    for (int i = 0; i < colorNames.size(); ++i) {
        auto* card = new ClickableFrame(colorNames[i], colorValues[i]);
        card->setObjectName("colorCard");
        card->setCursor(Qt::PointingHandCursor);

        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(8, 8, 8, 10);
        cardLayout->setSpacing(8);

        auto* swatch = new QFrame;
        swatch->setMinimumHeight(76);
        swatch->setStyleSheet(
            QString(
                "background-color: %1;"
                "border-radius: 9px;"
            ).arg(colorValues[i])
        );

        auto* name = new QLabel(colorNames[i]);
        name->setObjectName("colorName");

        auto* hex = new QLabel(colorValues[i].toUpper());
        hex->setObjectName("colorHex");

        cardLayout->addWidget(swatch);
        cardLayout->addWidget(name);
        cardLayout->addWidget(hex);

        colors->addWidget(card, i / 4, i % 4);
        colourCards.append(card);
    }

    editorLayout->addLayout(colors);
    editorLayout->addStretch();

    // Colour inspector
    auto* inspector = new QFrame;
    inspector->setObjectName("panel");
    inspector->setMinimumWidth(180);
    inspector->setMaximumWidth(250);

    auto* inspectorLayout = new QVBoxLayout(inspector);
    inspectorLayout->setContentsMargins(16, 18, 16, 16);
    inspectorLayout->setSpacing(14);

    auto* inspectorTitle = new QLabel("Colour details");
    inspectorTitle->setObjectName("panelTitle");

    auto* selectedColor = new QFrame;
    selectedColor->setMinimumHeight(110);
    selectedColor->setStyleSheet(
        "background-color: #89b4fa;"
        "border-radius: 12px;"
    );

    auto* selectedName = new QLabel("Blue");
    selectedName->setObjectName("panelTitle");

    auto* hexLabel = new QLabel("HEX VALUE");
    hexLabel->setObjectName("sectionTitle");

    auto* hexValue = new QLabel("#89B4FA");
    hexValue->setObjectName("valueField");

    auto* info = new QLabel(
        "Select a colour from the palette to inspect its properties."
    );
    info->setObjectName("mutedText");
    info->setWordWrap(true);

    inspectorLayout->addWidget(inspectorTitle);
    inspectorLayout->addWidget(selectedColor);
    inspectorLayout->addWidget(selectedName);
    inspectorLayout->addWidget(hexLabel);
    inspectorLayout->addWidget(hexValue);
    inspectorLayout->addWidget(info);
    inspectorLayout->addStretch();

    if (!colourCards.isEmpty())
    {
        colourCards.front()->setSelected(true);
    }

    for (auto* card : colourCards)
    {
        connect(card, &ClickableFrame::clicked, this,
            [card, colourCards, selectedName, hexValue, selectedColor]
            {
                for (auto* otherCard : colourCards)
                {
                    otherCard->setSelected(otherCard == card);
                }

                selectedName->setText(card->name());
                hexValue->setText(card->hex());

                selectedColor->setStyleSheet(QString("background-color: %1;"
                        "border-radius: 12px;").arg(card->hex())
                );
            }
        );
    }

    workspace->addWidget(sidebar);
    workspace->addWidget(editor, 1);
    workspace->addWidget(inspector);

    rootLayout->addLayout(workspace, 1);

    // Caelestia-inspired styling
    setStyleSheet(R"(
        QWidget#root {
            background-color: #111318;
            color: #e2e2eb;
            font-family: "Inter", "Noto Sans", sans-serif;
            font-size: 13px;
        }

        QFrame#colorCard[selected="true"] {
            border: 2px solid #cba6f7;
        }

        QLabel#appTitle {
            color: #e6e0f5;
            font-size: 21px;
            font-weight: 650;
        }

        QLabel#subtitle, QLabel#sectionTitle {
            color: #a9a4bb;
            font-size: 10px;
            font-weight: 700;
            letter-spacing: 1px;
        }

        QLabel#panelTitle {
            color: #e6e0f5;
            font-size: 17px;
            font-weight: 600;
        }

        QLabel#mutedText {
            color: #a4a2b3;
        }

        QFrame#panel {
            background-color: #191b22;
            border: 1px solid #2b2d38;
            border-radius: 16px;
        }

        QFrame#colorCard {
            background-color: #20222b;
            border: 1px solid #2b2d38;
            border-radius: 12px;
        }

        QLabel#colorName {
            color: #e2e0ec;
            font-weight: 600;
        }

        QLabel#colorHex {
            color: #a9a6b9;
            font-size: 11px;
        }

        QLabel#searchPlaceholder {
            color: #aaa6ba;
            background-color: #22242d;
            border: 1px solid #30323e;
            border-radius: 9px;
            padding: 10px;
        }

        QListWidget#schemeList {
            background: transparent;
            border: none;
            outline: none;
        }

        QListWidget#schemeList::item {
            color: #c4c1d2;
            padding: 11px 10px;
            border-radius: 9px;
            margin: 2px 0;
        }

        QListWidget#schemeList::item:selected {
            background-color: #373044;
            color: #e6dcff;
        }

        QListWidget#schemeList::item:hover:!selected {
            background-color: #252630;
        }

        QPushButton#primaryButton {
            background-color: #cba6f7;
            color: #211b2b;
            border: none;
            border-radius: 10px;
            padding: 11px 15px;
            font-weight: 650;
        }

        QPushButton#primaryButton:hover {
            background-color: #d8baff;
        }

        QLabel#valueField {
            background-color: #22242d;
            border: 1px solid #343642;
            border-radius: 9px;
            padding: 11px;
            color: #d8c5ff;
            font-family: monospace;
        }
    )");
}
