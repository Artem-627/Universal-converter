#include "App.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <strstream>

Window::Window() {
  this->create();
  this->setGeometry(100, 100, 500, 230);
  this->setWindowTitle("Converter");
  this->setStyleSheet("QMainWindow {"
                      "background-color: #352F44;"
                      "}");

  this->from_line = new QLineEdit(this);
  this->from_line->setStyleSheet("QLineEdit {"
                                 "background-color: #5C5470;"
                                 "border-radius: 10px;"
                                 "color: #FAF0E6;"
                                 "font-size: 25px;"
                                 "padding-left: 5px;"
                                 "}");
  this->from_line->setPlaceholderText("Enter a number");
  this->from_line->setGeometry(7, 10, 486, 40);
  this->from_line->show();

  this->from_label = new QLabel(this);
  this->from_label->setGeometry(17, 60, 70, 40);
  this->from_label->setStyleSheet("QLabel {"
                                  "color: #FAF0E6;"
                                  "font-size: 25px;"
                                  "}");
  this->from_label->setText("From");
  this->from_label->show();

  this->from_base = new QSpinBox(this);
  this->from_base->setStyleSheet(
      "QSpinBox {"
      "background-color: #5C5470;"
      "border-radius: 10px;"
      "color: #FAF0E6;"
      "font-size: 25px;"
      "padding-left: 5px;"
      "}"
      "QSpinBox::up-button {"
      "  image: url(../source/images/arrow-up.svg);"
      "  border-width: 0;"
      "}"
      "QSpinBox::down-button {"
      "  image: url(../source/images/arrow-down.svg);"
      "  border-width: 0;"
      "}");
  this->from_base->setMinimum(2);
  this->from_base->setMaximum(255);
  this->from_base->setValue(10);
  this->from_base->setGeometry(87, 60, 70, 40);
  this->from_base->show();

  this->to_label = new QLabel(this);
  this->to_label->setGeometry(167, 60, 70, 40);
  this->to_label->setStyleSheet("QLabel {"
                                "color: #FAF0E6;"
                                "font-size: 25px;"
                                "}");
  this->to_label->setText("to");
  this->to_label->show();

  this->to_base = new QSpinBox(this);
  this->to_base->setStyleSheet("QSpinBox {"
                               "background-color: #5C5470;"
                               "border-radius: 10px;"
                               "color: #FAF0E6;"
                               "font-size: 25px;"
                               "padding-left: 5px;"
                               "}"
                               "QSpinBox::up-button {"
                               "  image: url(../source/images/arrow-up.svg);"
                               "  border-width: 0;"
                               "}"
                               "QSpinBox::down-button {"
                               "  image: url(../source/images/arrow-down.svg);"
                               "  border-width: 0;"
                               "}");
  this->to_base->setMinimum(2);
  this->to_base->setMaximum(255);
  this->to_base->setValue(2);
  this->to_base->setGeometry(207, 60, 70, 40);
  this->to_base->show();

  this->button = new QPushButton(this);
  this->button->setGeometry(290, 60, 203, 40);
  this->button->setText("Convert");
  this->button->setStyleSheet("QPushButton {"
                              "background-color: #5C5470;"
                              "border-radius: 10px;"
                              "color: #FAF0E6;"
                              "font-size: 25px;"
                              "padding-left: 5px;"
                              "}");

  this->to_line = new QLineEdit(this);
  this->to_line->setStyleSheet("QLineEdit {"
                               "background-color: #5C5470;"
                               "border-radius: 10px;"
                               "color: #FAF0E6;"
                               "font-size: 25px;"
                               "padding-left: 5px;"
                               "}");
  this->to_line->setPlaceholderText("Result");
  this->to_line->setGeometry(7, 110, 486, 40);
  this->to_line->setReadOnly(1);
  this->to_line->show();

  this->open_button = new QPushButton(this);
  this->open_button->setGeometry(7, 180, 238, 40);
  this->open_button->setText("Open file");
  this->open_button->setStyleSheet("QPushButton {"
                                   "background-color: #5C5470;"
                                   "border-radius: 10px;"
                                   "color: #FAF0E6;"
                                   "font-size: 25px;"
                                   "padding-left: 5px;"
                                   "}");

  this->save_button = new QPushButton(this);
  this->save_button->setGeometry(255, 180, 238, 40);
  this->save_button->setText("Save file");
  this->save_button->setStyleSheet("QPushButton {"
                                   "background-color: #5C5470;"
                                   "border-radius: 10px;"
                                   "color: #FAF0E6;"
                                   "font-size: 25px;"
                                   "padding-left: 5px;"
                                   "}");

  QObject::connect(this->button, &QPushButton::clicked, [&]() {
    this->to_line->setText("");
    this->Calculate();
  });
  QObject::connect(this->open_button, &QPushButton::clicked,
                   [&]() { this->OpenFile(); });
  QObject::connect(this->save_button, &QPushButton::clicked,
                   [&]() { this->SaveFile(); });

  this->show();
};

void Window::Calculate() {
  uint8_t from_base_ = this->from_base->value();
  uint8_t to_base_ = this->to_base->value();
  std::string number_ = this->from_line->text().toStdString();

  // for (auto symbol = number_.begin(); symbol < number_.end(); ++symbol) {
  //   if (*symbol == '.' || *symbol == '(' || *symbol == ')' || *symbol == '['
  //   ||
  //       *symbol == ']') {
  //     continue;
  //   }
  //   if (!(*symbol >= '0' &&
  //         *symbol <= std::min((uint8_t)'9', (uint8_t)(from_base_ + '0' - 1)))
  //         &&
  //       !(*symbol >= 'A' && *symbol < 'A' + from_base_ - 10)) {
  //     this->to_line->setText("Incorrect number");
  //     return;
  //   }
  // }

  bool has_dot = 0;
  bool has_parenthesis = 0;
  for (auto symbol = number_.begin(); symbol < number_.end(); ++symbol) {
    if (*symbol == '.') {
      has_dot = 1;
    }
    if (*symbol == '(' || *symbol == ')') {
      has_parenthesis = 1;
    }
  }
  if (!has_dot) {
    number_ += ".0";
  }
  if (!has_parenthesis) {
    number_ += "(0)";
  }

  try {
    BigNumber num(number_, from_base_);

    this->to_line->setText(
        QString::fromStdString(num.Convert(to_base_).ToString()));
  } catch (const std::runtime_error &) {
    this->to_line->setText("Incorrect number");
  }
}

void Window::OpenFile() {
  QString file_name = QFileDialog::getOpenFileName(this, "Open File", "~", "*");
  std::string file = file_name.toStdString();

  std::ifstream fin(std::string(file.begin(), file.end()));
  std::string line;
  fin >> line;

  this->from_line->setText(QString::fromStdString(line));

  fin.close();
}

void Window::SaveFile() {
  QString file_name = QFileDialog::getSaveFileName(this, "Open File", "~", "*");
  std::string file = file_name.toStdString();

  std::ofstream fout(std::string(file.begin(), file.end()));
  fout << this->to_line->text().toStdString();

  fout.close();
}
