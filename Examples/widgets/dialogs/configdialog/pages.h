#ifndef PAGES_H
#define PAGES_H

#include <QtWidgets/QtWidgets>

class ConfigurationPage : public QWidget
{
	Q_OBJECT
public:
	ConfigurationPage(QWidget* parent = 0);
};

class QueryPage : public QWidget
{
	Q_OBJECT
public:
	QueryPage(QWidget* parent = 0);
};

class UpdatePage : public QWidget
{
	Q_OBJECT
public:
	UpdatePage(QWidget* parent = 0);
};

#endif