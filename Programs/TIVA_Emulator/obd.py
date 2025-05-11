import time
from collections import deque
from meters import *
from fault_manager import FaultManager

class CANProcessor:
    def __init__(self,fault_manager,max_messages=100):
        self.max_messages = max_messages
        self.can_messages = deque(maxlen=max_messages)
        self.buffer = bytearray()
        self.fault_manager = fault_manager

        self.engine_meter = None
        self.speed_meter = None
        self.pressure_gauge = None
        self.throttle_slider = None
        self.maf_meter = None
    
    def set_meters(self, engine_meter, speed_meter, pressure_gauge, throttle_slider, maf_meter):
        self.engine_meter = engine_meter
        self.speed_meter = speed_meter
        self.pressure_gauge = pressure_gauge
        self.throttle_slider = throttle_slider
        self.maf_meter = maf_meter
        
    def process_data(self, data, serial_handler=None):
        try:
            if isinstance(data, str):
                try:
                    data = bytes.fromhex(data.replace(' ', ''))
                except ValueError:
                    return
            
            self.buffer.extend(data)
            
            while len(self.buffer) >= 9:  # Minimum frame size
                if self.buffer[0] != 0xDF:
                    self.buffer.pop(0)
                    continue
                
                length = self.buffer[1]
                if len(self.buffer) < 2 + length:
                    break  # Wait for more data

                frame = self.buffer[:2 + length]
                del self.buffer[:2 + length]
                self._process_frame(frame, serial_handler)
        except Exception as e:
            print(f"CAN processing error: {e}")
            self.buffer.clear()

    def _process_frame(self, frame, serial_handler=None):
        """Process a complete CAN frame with DF 07 header appearing in the Serial """
        try:
            if len(frame) < 3 or frame[0] != 0xDF or frame[1] != 0x07:
                return
            
            length = frame [2]
            mode = frame[3]
            pid = frame[4]
            
            # Handle Mode 3 (Get DTCs)
            if mode == 0x03:
                print("Processing Mode: 03 (Get DTCs)")
                if serial_handler:
                    response = self._generate_dtc_response()
                    serial_handler.send(response)
                    print(f"Sent response: {response.hex(' ')}")
                return
            
            # Handle Mode 4 (Clear DTCs)
            elif mode == 0x04:
                print("Processing Mode: 04 (Clear DTCs)")
                if serial_handler:
                    response = self._generate_clear_dtc_response()
                    serial_handler.send(response)
                    print(f"Sent response: {response.hex(' ')}")
                return
            
            # Handle Mode 1 (Current Data)
            elif mode == 0x01:
                #pid = payload[1]
                print(f"Processing Mode: {mode:02X}, PID: {pid:02X}")
                if serial_handler:
                    response = self._generate_pid_response(mode, pid)
                    if response:
                        serial_handler.send(response)
                        print(f"Sent response: {response.hex(' ')}")
            
            # Store the message
            hex_frame = ' '.join(f"{b:02X}" for b in frame)
            self.can_messages.append(hex_frame)
            
        except Exception as e:
            print(f"Frame processing error: {e}")
            
    """Response generation for Mode 1 requests"""

    def _generate_pid_response(self, mode, pid):
        if not all([self.engine_meter, self.speed_meter, self.pressure_gauge,
                   self.throttle_slider, self.maf_meter]):
            print("Meter references not set")
            return None  
        
        if pid == 0x0C:  # Engine RPM (2 data bytes)
            rpm = self.engine_meter.current_value
            return bytes([0x07, 0xE8, 0x04, 0x41, pid, 
                         (rpm * 4) >> 8, (rpm * 4) & 0xFF, 0, 0, 0])
            
        elif pid == 0x0D:  # Vehicle Speed (1 data byte)
            speed = self.speed_meter.current_value
            return bytes([0x07, 0xE8, 0x03, 0x41, pid, speed, 0, 0, 0, 0])
            
        elif pid == 0x0A:  # Fuel Pressure (1 data byte)
            pressure = self.pressure_gauge.current_value
            return bytes([0x07, 0xE8, 0x03, 0x41, pid, 
                         int(pressure / 3), 0, 0, 0, 0])
            
        elif pid == 0x11:  # Throttle Position (1 data byte)
            throttle = self.throttle_slider.slider.get()
            return bytes([0x07, 0xE8, 0x03, 0x41, pid, 
                         int(throttle * 2.55), 0, 0, 0, 0])
            
        elif pid == 0x10:  # Mass Air Flow (2 data bytes)
            maf = self.maf_meter.current_value
            return bytes([0x07, 0xE8, 0x04, 0x41, pid, int(maf * 100) >> 8, int(maf * 100) & 0xFF, 0, 0, 0])
            
        print(f"Unsupported PID: {pid:02X}")
        return None
    
    def _generate_dtc_response(self):
        """Generate Mode 03 response"""
        dtc_codes = list(self.fault_manager.stored_dtc.keys())
        dtc_bytes = []
        
        # Convert DTC codes to bytes (max 3 DTCs = 6 bytes)
        for code in dtc_codes[:3]:
            if len(code) == 5:  # Format P0123
                first_char = code[0].upper()
                char_map = {'P': 0, 'C': 1, 'B': 2, 'U': 3}
                first_byte = char_map.get(first_char, 0)
                first_byte |= (int(code[1]) << 4)
                first_byte |= int(code[2])
                second_byte = (int(code[3]) << 4) | int(code[4])
                dtc_bytes.extend([first_byte, second_byte])
        
        # Build response (without DTC count byte)
        response = bytearray([0x07, 0xE8])  # Header
        response.append(1 + len(dtc_bytes))  # Length byte (mode + DTC bytes)
        response.append(0x43)  # Mode + 0x40
        response.extend(dtc_bytes)  # DTC data
        
        # Pad with zeros to make 10 bytes total
        while len(response) < 10:
            response.append(0x00)
        
        print(f"Generated DTC response: {response.hex(' ')}")  # Debug print
        return bytes(response)

    def _generate_clear_dtc_response(self):
        """Generate response for Mode 04 (Clear DTCs)"""
        self.fault_manager.clear_active()
        self.fault_manager.clear_stored()        
        return bytes([0x07, 0xE8, 0x01, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])

    def get_messages(self):
        return list(self.can_messages)