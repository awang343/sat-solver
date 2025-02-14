import time

class Timer:
    def __init__(self):
        self.start_time = 0
        self.stop_time = 0
        self.running = False

    def start(self):
        self.start_time = time.time()
        self.running = True

    def stop(self):
        if self.running:
            self.stop_time = time.time()
            self.running = False

    def get_time(self):
        if self.running:
            return time.time() - self.start_time
        return self.stop_time - self.start_time
