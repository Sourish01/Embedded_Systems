import serial
import serial.tools.list_ports
from threading import Thread
from queue import Queue
from datetime import datetime
import sys
import time

class SerialHandler:
    def __init__(self, baudrate=115200):
        self.baudrate = baudrate
        self.rx_queue = Queue()  # Incoming data
        self.tx_queue = Queue()  # Outgoing data
        self.serial_thread = None
        self._running = False
        self.buffer = bytearray()  # For assembling frames
        self.frame_size = 10  # Fixed 10-byte frame size
        self.port = None
    
    """ Function for starting the serial thread."""    
    def start(self):
        if not self._running:
            self._running = True
            self.serial_thread = Thread(target=self._run_serial, daemon=True)
            self.serial_thread.start()
            self._log("Serial handler started")
            
    """ Function for stopping the serial thread."""    
    def stop(self):
        if self._running:
            self._running = False
            if self.serial_thread:
                self.serial_thread.join(timeout=1)
            if self.port and self.port.is_open:
                self.port.close()
            self._log("Serial handler stopped")
            
    """ Function to send the data""" 
    def send(self, data):
        if isinstance(data, (bytes, bytearray)):
            self.tx_queue.put(data)
            self._log(f"Queued for TX: {data.hex(' ')}")
        else:
            self._log(f"Invalid data type for transmission: {type(data)}")
            
            
    """ COM Port Connection and Exception Handling""" 

    def _run_serial(self):
        while self._running:
            try:
                if self.port is None or not self.port.is_open:
                    self._connect_serial()
                    if self.port is None:
                        time.sleep(1)
                        continue
                
                self._handle_serial_io()
                
            except Exception as e:
                self._log(f"Serial error: {str(e)}")
                if self.port:
                    self.port.close()
                self.port = None
                time.sleep(1)

    def _connect_serial(self):
        """Try to connect to available serial ports"""
        ports = serial.tools.list_ports.comports()
        for port in ports:
            try:
                self.port = serial.Serial(
                    port.device,
                    self.baudrate,
                    timeout=0.1,
                    write_timeout=0.1,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE
                )
                self._log(f"Connected to {port.device}")
                return True
            except Exception as e:
                self._log(f"Failed to connect to {port.device}: {str(e)}")
        return False

    """Handling incoming and outcoming data"""
    def _handle_serial_io(self):
        # Process incoming data
        if self.port.in_waiting > 0:
            data = self.port.read(self.port.in_waiting)
            self._log(f"RX Raw: {data.hex(' ')}")
            self._process_incoming_data(data)
        
        # Process outgoing data
        self._process_outgoing_data()

    """Process incoming data with strict 10-byte frame handling"""
    def _process_incoming_data(self, data):
        self.buffer.extend(data)
        
        while len(self.buffer) >= self.frame_size:
            # Find start of frame (0xDF)
            try:
                start_pos = self.buffer.index(0xDF)
                if start_pos > 0:
                    self._log(f"Discarding {start_pos} bytes before frame start")
                    self.buffer = self.buffer[start_pos:]
            except ValueError:
                # No start byte found, clear buffer
                self.buffer.clear()
                return
                
            # Check if we have a complete frame
            if len(self.buffer) < self.frame_size:
                return
                
            # Extract the complete 10-byte frame
            frame = self.buffer[:self.frame_size]
            self.buffer = self.buffer[self.frame_size:]
            
            # Validate basic frame structure
            if frame[0] != 0xDF or frame[1] != 0x07:
                self._log(f"Invalid frame header: {frame[:2].hex(' ')}")
                continue
                
            # Queue the valid frame for processing
            self.rx_queue.put(bytes(frame))
            self._log(f"RX Frame: {frame.hex(' ')}")

    def _process_outgoing_data(self):
        """Send queued data with proper framing"""
        while not self.tx_queue.empty():
            data = self.tx_queue.get_nowait()
            try:
                if isinstance(data, (bytes, bytearray)):
                    # Ensure proper 10-byte frame
                    if len(data) != self.frame_size:
                        self._log(f"Invalid frame length: {len(data)} bytes")
                        continue
                        
                    self.port.write(data)
                    self._log(f"TX Frame: {data.hex(' ')}")
                else:
                    self._log(f"Invalid data type for transmission: {type(data)}")
            except Exception as e:
                self._log(f"TX Error: {str(e)}")
                self.tx_queue.put(data)  # Requeue failed message
                break

    def _log(self, message):
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        print(f"[Serial @ {timestamp}] {message}", file=sys.stderr)
        sys.stderr.flush()

    def has_data(self):
        """Check if there's data available"""
        return not self.rx_queue.empty()

    def get_data(self):
        """Get received data"""
        try:
            return self.rx_queue.get_nowait()
        except:
            return None