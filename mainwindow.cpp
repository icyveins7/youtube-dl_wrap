#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dirBtn_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, false);
    ui->dirEdit->setText(dialog.getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath()));
}

void MainWindow::on_dlBtn_clicked()
{
    QString link = ui->linkEdit->text();

    std::string cmd = "youtube-dl.exe " + link.toStdString();

    // get path of executable
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    PathRemoveFileSpecA(path);

    cmd = std::string(path) + std::string("\\") + cmd;
    cmd = cmd + std::string(" -o \"") + ui->dirEdit->text().toStdString() + std::string("\\%(title)s.%(ext)s\" -f best");

//    std::cout<<cmd<<std::endl; // for debugging

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
        std::cout<<buffer.data();
    }
}
