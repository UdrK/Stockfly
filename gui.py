import chess
import chess.svg

def save_svg(fen):
    # writing SVG
    board = chess.Board(fen)
    boardsvg = chess.svg.board(board, size=350)
    with open('test.svg', "w") as outputfile:
        outputfile.write(boardsvg)
        print("Successfully written SVG to file")

from PyQt5 import QtGui, QtSvg
from PyQt5.QtCore import Qt, QObject, QThread, pyqtSignal
from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QMainWindow,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

import socket

class Antenna(QObject):
    finished = pyqtSignal()
    received = pyqtSignal(str)

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST, PORT))
            s.listen()

            print("Socket ready, Waiting for client [..]")

            conn, addr = s.accept()

            with conn:
                print("Successfully connected with client [.]")

                data = conn.recv(1024)  # receiving data from engine
                fen = data.decode("utf-8")  # preparing for interpretation

                while fen != "Game Ended":
                    print("Waiting for communications...")

                    data = conn.recv(1024)  # receiving data from engine
                    fen = data.decode("utf-8")  # preparing for interpretation

                    print("Received: \n{}".format(fen))

                    if fen == "Game Ended":
                        break
                    else:
                        save_svg(fen)
                        self.received.emit(fen)

                self.finished.emit()

class Window(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi()

    def setupUi(self):
        self.setWindowTitle("Server GUI")
        self.resize(500, 500)
        self.centralWidget = QtSvg.QSvgWidget()
        self.setCentralWidget(self.centralWidget)
        self.run_antenna()

    def update_gui(self):
        self.centralWidget.load('test.svg')

    def run_antenna(self):
        self.thread = QThread()

        self.antenna = Antenna()

        self.antenna.moveToThread(self.thread)

        self.thread.started.connect(self.antenna.run)
        self.thread.finished.connect(self.thread.quit)
        self.antenna.finished.connect(self.antenna.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)
        self.antenna.received.connect(self.update_gui)

        self.thread.start()

import sys

HOST = "127.0.0.1"
PORT = 24718            # Port to listen on, C++ program will write on this port

app = QApplication(sys.argv)
w = Window()
w.show()
sys.exit(app.exec())