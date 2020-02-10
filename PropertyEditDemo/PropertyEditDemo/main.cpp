#include "propertyeditdemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PropertyEditDemo w;
	w.showMaximized ();
	return a.exec();
}
