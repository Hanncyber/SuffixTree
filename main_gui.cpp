#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application style
    app.setStyle("Fusion");
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
