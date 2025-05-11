import time
from dtc_definitions import DTC_DATABASE
            
class FaultManager:
    def __init__(self):
        self.active_dtc = {}
        self.stored_dtc = {}
        self.max_stored_dtc = 3  # Maximum stored DTCs

    def inject_fault(self, code):
        if code in DTC_DATABASE:
            self.active_dtc[code] = {"timestamp": time.time(), **DTC_DATABASE[code]}

    def clear_active(self):
        self.active_dtc.clear()

    def clear_stored(self):
        self.stored_dtc.clear()

    def move_to_stored(self, code):
        if code in self.active_dtc:
            # Enforce maximum stored DTCs
            if len(self.stored_dtc) >= self.max_stored_dtc:
                # Remove oldest DTC
                oldest_code = min(self.stored_dtc.items(), 
                                key=lambda x: x[1]['timestamp'])[0]
                del self.stored_dtc[oldest_code]
            
            self.stored_dtc[code] = self.active_dtc.pop(code)