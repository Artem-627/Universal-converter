#pragma once

#include "BigNumber.h"
#include <QApplication>
#include <QBoxLayout>
#include <QDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QRect>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class Window : QMainWindow {
public:
  Window();

private:
  QWidget *window;
  QPushButton *button;

  QLabel *from_label;
  QLabel *to_label;
  QSpinBox *from_base;
  QSpinBox *to_base;
  QLineEdit *from_line;
  QLineEdit *to_line;

  QPushButton *open_button;
  QPushButton *save_button;

  void OpenFile();
  void SaveFile();

  void Calculate();
};
