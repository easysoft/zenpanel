#ifndef GLOBALCONTROL_H
#define GLOBALCONTROL_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

#include "controller.h"
#include "base/gscale.h"
#include "base/glang.h"

class GlobalControl : public QWidget,GScale,GLang
{
};

#endif // GLOBALCONTROL_H
