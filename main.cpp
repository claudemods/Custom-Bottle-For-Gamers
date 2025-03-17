#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QMenu>
#include <QInputDialog>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>

class PasswordDialog : public QDialog {
    Q_OBJECT

public:
    PasswordDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Enter Password");

        QFormLayout *layout = new QFormLayout(this);

        passwordLineEdit = new QLineEdit(this);
        passwordLineEdit->setEchoMode(QLineEdit::Password); // Hide password input
        layout->addRow("Password:", passwordLineEdit);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        layout->addRow(buttonBox);
    }

    QString getPassword() const {
        return passwordLineEdit->text();
    }

private:
    QLineEdit *passwordLineEdit;
};

class CustomBottleInstaller : public QWidget {
    Q_OBJECT

public:
    CustomBottleInstaller(QWidget *parent = nullptr) : QWidget(parent) {
        // Set up the main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Create a single QLabel to display both the image and text
        QLabel *logoTextLabel = new QLabel(this);
        QPixmap logo(":/images/logo.png"); // Load logo from resources
        if (logo.isNull()) {
            QMessageBox::warning(this, "Error", "Failed to load logo from resources.");
        } else {
            // Use HTML to display the image and text in a single QLabel
            logoTextLabel->setText(QString(
                "<div style='text-align: center;'>"
                "<img src=':/images/logo.png'><br/>" // Display image as-is
                "<span style='color: teal; font-size: 20px; font-weight: bold; margin: 0; padding: 0;'>"
                "Custom Bottle For Gamers Installer v1.0"
                "</span>"
                "</div>"
            ));
            logoTextLabel->setAlignment(Qt::AlignCenter);
        }

        // Add the logo and text label to the main layout
        mainLayout->addWidget(logoTextLabel);

        // Add progress bar (left-to-right with percentage)
        progressBar = new QProgressBar(this);
        progressBar->setStyleSheet(
            "QProgressBar {"
            "   border: 2px solid teal;"
            "   border-radius: 5px;"
            "   background-color: gold;"
            "   text-align: center;"
            "   color: gold;" // Set percentage text color to gold
            "   margin-top: 10px;" // Add some space above the progress bar
            "}"
            "QProgressBar::chunk {"
            "   background-color: teal;"
            "   width: 10px;"
            "}"
        );
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        progressBar->setFormat("%p%"); // Show percentage
        mainLayout->addWidget(progressBar);

        // Add Download button (opens submenu)
        QPushButton *downloadButton = new QPushButton("Download", this);
        downloadButton->setStyleSheet("background-color: teal; color: gold; font-size: 16px; padding: 10px;");
        connect(downloadButton, &QPushButton::clicked, this, &CustomBottleInstaller::showDownloadMenu);
        mainLayout->addWidget(downloadButton);

        // Add Install button (opens submenu)
        QPushButton *installButton = new QPushButton("Install", this);
        installButton->setStyleSheet("background-color: teal; color: gold; font-size: 16px; padding: 10px;");
        connect(installButton, &QPushButton::clicked, this, &CustomBottleInstaller::showInstallMenu);
        mainLayout->addWidget(installButton);

        // Set gold background
        setStyleSheet("background-color: gold;");

        // Set window title and size
        setWindowTitle("Custom Bottle Installer");
        resize(800, 600); // Larger window to accommodate bigger logo
    }

private slots:
    void showDownloadMenu() {
        QMenu downloadMenu(this);

        QAction *downloadSquashfsAction = downloadMenu.addAction("Download Squashfs");
        QAction *downloadDwarfsAction = downloadMenu.addAction("Download Dwarfs");

        connect(downloadSquashfsAction, &QAction::triggered, this, &CustomBottleInstaller::downloadSquashfs);
        connect(downloadDwarfsAction, &QAction::triggered, this, &CustomBottleInstaller::downloadDwarfs);

        downloadMenu.exec(QCursor::pos());
    }

    void showInstallMenu() {
        QMenu installMenu(this);

        QAction *installSquashfsAction = installMenu.addAction("Install Squashfs");
        QAction *installDwarfsAction = installMenu.addAction("Install Dwarfs");

        connect(installSquashfsAction, &QAction::triggered, this, &CustomBottleInstaller::installSquashfs);
        connect(installDwarfsAction, &QAction::triggered, this, &CustomBottleInstaller::installDwarfs);

        installMenu.exec(QCursor::pos());
    }

    void downloadSquashfs() {
        progressBar->setValue(50); // Jump to 50%
        QString downloadLink = "https://drive.usercontent.google.com/download?id=1DLdKIWtxWNsELkEmftBEE_zesEnkBmhq&export=download&authuser=0&confirm=t&uuid=fe218f1b-6966-4165-9ee8-138f66c4589f&at=AEz70l6MSuC23U7OYnSxdjGZqPHJ:1742109696632";
        QString outputFilePath = QDir::currentPath() + "/GamingBottle.squashfs";
        downloadFile(downloadLink, outputFilePath);
    }

    void downloadDwarfs() {
        progressBar->setValue(50); // Jump to 50%
        QString downloadLink = "https://drive.usercontent.google.com/download?id=1j6XkEkaEnqOHj-1qtXvhy_F9ehSTTa5Z&export=download&authuser=0&confirm=t&uuid=8c19db4e-99f6-4981-92b0-d03db51bb46d&at=AEz70l7BxNJoQI7a0P0nSYBj1P6f:1742109796196";
        QString outputFilePath = QDir::currentPath() + "/GamingBottle.dwarfs";
        downloadFile(downloadLink, outputFilePath);
    }

    void installSquashfs() {
        progressBar->setValue(50); // Jump to 50%
        QString filePath = QDir::currentPath() + "/GamingBottle.squashfs";
        QString extractPath = QDir::homePath() + "/.var"; // Extract to ~/.var

        if (QFile::exists(filePath)) {
            QProcess process;
            QString extractCommand = QString("unsquashfs -f -d %1 %2").arg(extractPath).arg(filePath);
            process.start("sh", QStringList() << "-c" << extractCommand);

            process.waitForFinished();

            if (process.exitCode() == 0) {
                progressBar->setValue(100); // Jump to 100%
                QMessageBox::information(this, "Installation Complete", "Squashfs extraction completed successfully!");
            } else {
                QMessageBox::warning(this, "Installation Failed", "Failed to extract the .squashfs file. Check permissions and file integrity.");
            }
        } else {
            QMessageBox::warning(this, "File Not Found", "GamingBottle.squashfs not found in the current directory.");
        }
    }

    void installDwarfs() {
        QMessageBox::warning(this, "Not Supported", "Dwarfs installation is not supported in this version.");
    }

private:
    QProgressBar *progressBar;

    void downloadFile(const QString &url, const QString &outputFilePath) {
        QProcess *process = new QProcess(this);
        connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
            QString output = process->readAllStandardOutput();
            if (output.contains("%")) {
                QString percentageStr = output.split(" ").at(6); // Extract the percentage string
                percentageStr.replace("%", ""); // Remove the "%" character
                int percentage = percentageStr.toInt(); // Convert to integer
                progressBar->setValue(percentage); // Update the progress bar
            }
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, outputFilePath](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
                progressBar->setValue(100); // Jump to 100%
                QMessageBox::information(this, "Download Complete", "File downloaded successfully!");
            } else {
                QMessageBox::warning(this, "Download Failed", "Failed to download the file.");
            }
        });

        process->start("wget", QStringList() << "--no-check-certificate" << url << "-O" << outputFilePath);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CustomBottleInstaller installer;
    installer.show();

    return app.exec();
}

#include "main.moc"
