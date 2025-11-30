from PyQt5.QtWidgets import QMainWindow, QPushButton, QLabel, QWidget, QVBoxLayout, QDesktopWidget, QHBoxLayout
from PyQt5.QtCore import Qt

class Matching_ui(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Least-Square Matching Application")
        self.resize(800, 600)  # 设置大小
        qt_rectangle = self.frameGeometry()
        center_point = QDesktopWidget().availableGeometry().center()
        qt_rectangle.moveCenter(center_point)
        self.move(qt_rectangle.topLeft())

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout()
        central_widget.setLayout(main_layout)

        img_layout = QHBoxLayout()
        self.left_img = QLabel("Left Image")
        self.left_img.setFixedSize(350, 200)
        self.left_img.setAlignment(Qt.AlignCenter)

        self.right_img = QLabel("Right Image")
        self.right_img.setFixedSize(350, 200)
        self.right_img.setAlignment(Qt.AlignCenter)

        img_layout.addWidget(self.left_img)
        img_layout.addWidget(self.right_img)

        main_layout.addLayout(img_layout)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.update_label_font()

    def update_label_font(self):
        for label in [self.left_img, self.right_img]:
            h = label.height()
            font_size = max(8, int(h * 0.10))
            label.setStyleSheet(
                f"border:1px solid gray; font-family:Consolas; font-size:{font_size}px;"
            )
