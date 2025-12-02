from python.app.ui import Matching_ui as UI
from PyQt5.QtWidgets import QApplication, QSplashScreen, QFileDialog, QMessageBox
import sys
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt
from lsmatching import Matching
import os

def show_info(parent, title="提示", message="操作成功"):
    msg_box = QMessageBox(parent)
    msg_box.setWindowTitle(title)
    msg_box.setText(message)
    msg_box.setIcon(QMessageBox.Information)
    msg_box.setStandardButtons(QMessageBox.Ok)
    msg_box.exec_()

class matching_app:
    working_directory = None
    left_img_path = None
    right_img_path = None
    left_points = None
    right_points = None

    def __init__(self):
        self.app = QApplication(sys.argv)
        self.ui = UI()
        self.ui.show()
        self.ui.directory_button.clicked.connect(self.set_directory)
        self.ui.left_btn.clicked.connect(self.get_left_image)
        self.ui.right_btn.clicked.connect(self.get_right_image)
        self.ui.choose_btn.clicked.connect(self.choose_cal)
        self.ui.matching_btn.clicked.connect(self.matching_cal)
        self.ui.output_btn.clicked.connect(self.matching_output)

    def get_points(self):
        left_path = os.path.join(self.working_directory, "Left_result.dat")
        right_path = os.path.join(self.working_directory, "Right_result.dat")
        self.left_points = []
        self.right_points = []
        self.right_points_corrected = []
        with open(left_path, 'r') as file:
            for line in file:
                line = line.strip()
                if line:
                    x, y = line.split('\t')
                    self.left_points.append((int(x), int(y)))
        with open(right_path, 'r') as file:
            for line in file:
                line = line.strip()
                if line:
                    x, y = line.split('\t')
                    self.right_points.append((int(x), int(y)))

    def set_directory(self):
        folder = QFileDialog.getExistingDirectory(self.ui, "选择工作空间")
        if folder:
            self.working_directory = folder
            self.ui.directory_label.setText(folder)

    def get_left_image(self):
        if not self.working_directory:
            show_info(self.ui, title = "提示", message= "请先选择工作空间！")
            return
        self.left_img_path, _ = QFileDialog.getOpenFileName(
                                                            self.ui,
                                                            "选择左影像",
                                                            self.working_directory,
                                                            "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"
                                                        )
        if not self.left_img_path:
            return
        self.ui.left_img.set_image(self.left_img_path)

    def get_right_image(self):
        if not self.working_directory:
            show_info(self.ui, title = "提示", message= "请先选择工作空间！")
            return
        self.right_img_path, _ = QFileDialog.getOpenFileName(
                                                            self.ui,
                                                            "选择右影像",
                                                            self.working_directory,
                                                            "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"
                                                        )
        if not self.right_img_path:
            return
        self.ui.right_img.set_image(self.right_img_path)

    def choose_cal(self):
        if self.ui.left_img.click_point is None or self.ui.right_img.click_point is None:
            show_info(self.ui, message="请先选择同名点！")
            return
        matching = Matching(self.left_img_path, self.right_img_path)
        x1, y1 = self.ui.left_img.click_point
        x2, y2 = self.ui.right_img.click_point
        matching.set_params(35)
        matching.set_centers(x1, y1, x2, y2)
        left_window = matching.get_left_window()
        right_window = matching.get_right_window()
        matching.calculate()
        left_window_matched = matching.get_left_window()
        right_window_matched = matching.get_right_window()

        self.ui.left_window_origin.set_image(left_window)
        self.ui.right_window_origin.set_image(right_window)
        self.ui.left_window.set_image(left_window_matched)
        self.ui.right_window.set_image(right_window_matched)

    def matching_cal(self):
        matching = Matching(self.left_img_path, self.right_img_path)
        matching.set_params(35)
        matching.get_matched_points(self.working_directory)
        self.get_points()
        for index, _ in enumerate(self.left_points):
            x1, y1 = self.left_points[index]
            x2, y2 = self.right_points[index]
            matching.set_centers(x1, y1, x2, y2)
            matching.calculate()
            self.right_points_corrected.append((matching.get_matched_x(), matching.get_matched_y()))

    def matching_output(self):
        corrected_y_path = os.path.join(self.working_directory, "Right_result_corrected.dat")
        with open(corrected_y_path, 'w') as file:
            for (x, y) in self.right_points_corrected:
                file.write(f"{x}\t{y}\n")
    
    def run(self):
        sys.exit(self.app.exec_())