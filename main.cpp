#include <QApplication>
#include <QWidget>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Qt5 Simple Demo");
    window.resize(300, 200);

    QLabel *label = new QLabel("Hello, Qt5!", &window);
    label->move(100, 80);

    window.show();

    return app.exec();
}
