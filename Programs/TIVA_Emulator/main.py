import customtkinter as ctk
from home_page import HomePage
from dial_page import DialPage
from fault_page import FaultPage  # Add this import
from serial_handler import SerialHandler
from obd import CANProcessor
from fault_manager import FaultManager
import threading
import queue

class App(ctk.CTk):
    def __init__(self):
        super().__init__()
        
        # Configure window first
        self.title("Car Emulator")
        self.geometry("1000x700")
        self.minsize(800, 600)
        
        # Set theme
        ctk.set_appearance_mode("Dark")
        ctk.set_default_color_theme("blue")
        
        # Initialize components
        self._initialize_components()
        
        # Center window
        self.center_window()
        
        # Start serial checking AFTER full initialization
        self._start_serial_checking()
        
        # Bind window close event
        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def _initialize_components(self):
        """Initialize all components"""
        # Serial handler
        self.serial = SerialHandler()
        self.serial.start()
        
        self.fault_manager = FaultManager()
        
        # CAN Processor
        self.can_processor = CANProcessor(self.fault_manager)
        
        # Container for pages
        self.container = ctk.CTkFrame(self)
        self.container.pack(side="top", fill="both", expand=True)
        self.container.grid_rowconfigure(0, weight=1)
        self.container.grid_columnconfigure(0, weight=1)
        
        # Initialize pages - ADDED FAULTPAGE HERE
        self.frames = {
            "HomePage": HomePage(self.container, self),
            "DialPage": DialPage(self.container, self, self.can_processor,self.serial),
            "FaultPage": FaultPage(self.container, self,self.fault_manager,self.can_processor,self.serial)
        }
        
        # Grid all frames
        for frame in self.frames.values():
            frame.grid(row=0, column=0, sticky="nsew")
        
        self.show_page("HomePage")

    def _start_serial_checking(self):
        """Start serial checking after full initialization"""
        if hasattr(self, 'after'):
            self.after_id = self.after(100, self._safe_check_serial)

    def _safe_check_serial(self):
        """Wrapper to safely handle serial checks"""
        try:
            if hasattr(self, 'check_serial'):
                self.check_serial()
        except Exception as e:
            print(f"Serial check error: {e}")
        finally:
            if hasattr(self, 'after'):
                self.after_id = self.after(100, self._safe_check_serial)

    def center_window(self):
        self.update_idletasks()
        width = self.winfo_width()
        height = self.winfo_height()
        x = (self.winfo_screenwidth() // 2) - (width // 2)
        y = (self.winfo_screenheight() // 2) - (height // 2)
        self.geometry(f'{width}x{height}+{x}+{y}')

    def show_page(self, page_name):
        """Show the specified page"""
        frame = self.frames.get(page_name)
        if frame:
            frame.tkraise()
        else:
            print(f"Error: Page '{page_name}' not found in frames")
        
    def check_serial(self):
        """Periodically check for incoming serial data"""
        if not hasattr(self, 'serial') or not self.serial:
            return
            
        while self.serial.has_data():
            data = self.serial.get_data()
            if data:
                print(f"Processing: {data.hex(' ')}")
                self.can_processor.process_data(data, self.serial)
                
                current_page = self.get_current_page()
                if hasattr(current_page, 'process_serial_data'):
                    current_page.process_serial_data(data)
        
    def get_current_page(self):
        for frame in self.frames.values():
            if frame.winfo_ismapped():
                return frame
        return None
        
    def send_command(self, command):
        if hasattr(self, 'serial') and self.serial:
            self.serial.send(command)
        
    def on_close(self):
        """Clean up when closing"""
        if hasattr(self, 'after_id'):
            self.after_cancel(self.after_id)
        if hasattr(self, 'serial'):
            self.serial.stop()
        self.destroy()

if __name__ == "__main__":
    app = App()
    app.mainloop()