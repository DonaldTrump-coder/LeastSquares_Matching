from ._lsmatching import Matching as LSMatching
import numpy as np

class Matching:
    def __init__(self, left_image_path: str, right_image_path: str):
        self.matching = LSMatching(left_image_path, right_image_path)
    
    def set_params(self, windowsize: int = 15, d_corr_threshold: float = 0.04):
        self.matching.set_params(int(windowsize), d_corr_threshold)

    def set_matching_params(self, windowsize: int = 3, corr_threshold: float = 0.7):
        self.matching.set_matching_params(int(windowsize), corr_threshold)

    def set_centers(self, x1: int, y1: int, x2: int, y2: int):
        self.matching.set_centers(int(x1), int(y1), int(x2), int(y2))

    def calculate(self):
        self.matching.calculate()

    def get_left_window(self) -> np.ndarray:
        return self.matching.get_left_window()
    
    def get_right_window(self) -> np.ndarray:
        return self.matching.get_right_window()
    
    def get_matched_points(self, savepath: str):
        self.matching.get_matched_points(savepath)

    def get_matched_x(self) -> float:
        return self.matching.get_matched_x()

    def get_matched_y(self) -> float:
        return self.matching.get_matched_y()
    
    def get_delta0(self) -> float:
        return self.matching.get_delta0()
    
    def get_SNR(self) -> float:
        return self.matching.get_SNR()
    
    def get_rho(self) -> float:
        return self.matching.get_rho()
    
    def get_deltag(self) -> float:
        return self.matching.get_deltag()
    
    def get_deltag_(self) -> float:
        return self.matching.get_deltag_()
    
    def get_deltax(self) -> float:
        return self.matching.get_deltax()
    
    def get_h0(self) -> float:
        return self.matching.get_h0()
    
    def get_h1(self) -> float:
        return self.matching.get_h1()
    
    def get_a0(self) -> float:
        return self.matching.get_a0()
    
    def get_a1(self) -> float:
        return self.matching.get_a1()
    
    def get_a2(self) -> float:
        return self.matching.get_a2()
    
    def get_b0(self) -> float:
        return self.matching.get_b0()
    
    def get_b1(self) -> float:
        return self.matching.get_b1()
    
    def get_b2(self) -> float:
        return self.matching.get_b2()