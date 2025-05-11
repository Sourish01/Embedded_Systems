from tkdial import Meter
import customtkinter as ctk

class EngineSpeedMeter(ctk.CTkFrame):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self.configure(fg_color="transparent")
        
        # Create engine speed meter (0-16384 RPM)
        self.meter = Meter(
            self,
            radius=250,
            start=0,
            end=16384,
            border_width=4,
            fg="black",
            bg="#1a1a1a",
            text_color="white",
            start_angle=220,
            end_angle=-320,
            text_font=("Arial", 16),
            scale_color="white",
            needle_color="#ff3366",
            scroll_steps=1000  # Changed to match 1000 RPM increments
        )
        self.current_value = 0
        self.meter.set(self.current_value)
        self.meter.pack(pady=15)
        
        # Title label
        title_label = ctk.CTkLabel(
            self,
            text="ENGINE SPEED",
            font=("Arial", 20, "bold"),
            text_color="white"
        )
        title_label.pack(pady=(0, 5))
        
        # Digital display        
        self.value_label = ctk.CTkLabel(
            self,
            text=f"{self.current_value} RPM",
            font=("Arial", 28, "bold"),
            text_color="#ff3366"  # Matches needle color
        )
        self.value_label.pack(pady=(0, 15))
        self.meter.bind("<ButtonRelease-1>", lambda e: self.on_change())
        self.meter.bind("<B1-Motion>", lambda e: self.on_change())

    def increase(self):
        new_value = min(16384, self.current_value + 1000)  # Changed to 1000
        self.set_value(new_value)

    def decrease(self):
        new_value = max(0, self.current_value - 1000)  # Changed to 1000
        self.set_value(new_value)

    def set_value(self, value):
        self.current_value = value
        self.meter.set(self.current_value)
        self.value_label.configure(text=f"{self.current_value} RPM")
        print(f"Engine speed: {self.current_value} RPM")

    def on_change(self):
        current = self.meter.get()
        rounded = round(current / 1000) * 1000  # Round to nearest 1000
        if rounded != self.current_value:
            self.set_value(rounded)


class VehicleSpeedMeter(ctk.CTkFrame):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self.configure(fg_color="transparent")
        
        # Create vehicle speed meter (0-255 km/h)
        self.meter = Meter(
            self,
            radius=250,
            start=0,
            end=255,
            border_width=4,
            fg="black",
            bg="#1a1a1a",
            text_color="white",
            start_angle=220,
            end_angle=-320,
            text_font=("Arial", 16),
            scale_color="white",
            needle_color="#42a5f5",
            scroll_steps=5  # Changed to match 5 km/h increments
        )
        self.current_value = 0
        self.meter.set(self.current_value)
        self.meter.pack(pady=15)
        
        # Title label
        title_label = ctk.CTkLabel(
            self,
            text="VEHICLE SPEED",
            font=("Arial", 20, "bold"),
            text_color="white"
        )
        title_label.pack(pady=(0, 5))
        
        # Digital display        
        self.value_label = ctk.CTkLabel(
            self,
            text=f"{self.current_value} km/h",
            font=("Arial", 28, "bold"),
            text_color="#42a5f5"  # Matches needle color
        )
        self.value_label.pack(pady=(0, 15))
        self.meter.bind("<ButtonRelease-1>", lambda e: self.on_change())
        self.meter.bind("<B1-Motion>", lambda e: self.on_change())

    def increase(self):
        new_value = min(255, self.current_value + 5)  # Changed to 5
        self.set_value(new_value)

    def decrease(self):
        new_value = max(0, self.current_value - 5)  # Changed to 5
        self.set_value(new_value)

    def set_value(self, value):
        self.current_value = value
        self.meter.set(self.current_value)
        self.value_label.configure(text=f"{self.current_value} km/h")
        print(f"Vehicle speed: {self.current_value} km/h")

    def on_change(self):
        current = self.meter.get()
        rounded = round(current / 5) * 5  # Round to nearest 5
        if rounded != self.current_value:
            self.set_value(rounded)

class PressureGauge(ctk.CTkFrame):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self.configure(fg_color="transparent")
        
        # Create pressure meter
        self.meter = Meter(
            self,
            radius=250,
            start=0,
            end=765,
            border_width=4,
            fg="black",
            bg="#1a1a1a",
            text_color="white",
            start_angle=220,
            end_angle=-320,
            text_font=("Arial", 16),
            scale_color="white",
            needle_color="#4CAF50",
            scroll_steps=50
        )
        self.current_value = 0
        self.meter.set(self.current_value)
        self.meter.pack(pady=15)
        
        # Title label
        title_label = ctk.CTkLabel(
            self,
            text="FUEL PRESSURE",
            font=("Arial", 20, "bold"),
            text_color="white"
        )
        title_label.pack(pady=(0, 5))
        
        # Digital display
        self.value_label = ctk.CTkLabel(
            self,
            text=f"{self.current_value} kPa",
            font=("Arial", 28, "bold"),
            text_color="#4CAF50"
        )
        self.value_label.pack(pady=(0, 15))
        self.meter.bind("<ButtonRelease-1>", lambda e: self.on_change())
        self.meter.bind("<B1-Motion>", lambda e: self.on_change())

    def increase(self):
        new_value = min(765, self.current_value + 50)
        self.set_value(new_value)

    def decrease(self):
        new_value = max(0, self.current_value - 50)
        self.set_value(new_value)

    def set_value(self, value):
        self.current_value = value
        self.meter.set(self.current_value)
        self.value_label.configure(text=f"{self.current_value} kPa")
        print(f"Fuel pressure: {self.current_value} kPa")

    def on_change(self):
        current = self.meter.get()
        rounded = round(current / 50) * 50
        if rounded != self.current_value:
            self.set_value(rounded)
                    
                    
class ThrottlePositionSlider(ctk.CTkFrame):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self.configure(fg_color="transparent")
                
        # Main content frame
        content_frame = ctk.CTkFrame(self, fg_color="transparent")
        content_frame.pack(pady=10)
        
        # Vertical slider
        self.slider = ctk.CTkSlider(
            content_frame,
            from_=0,
            to=100,
            number_of_steps=100,
            width=20,
            height=200,
            orientation="vertical",
            button_color="#FFA500",
            button_hover_color="#FFC04D",
            progress_color=("#1a1a1a", "#FFA500"),
            fg_color="#333333",
            command=self.on_slide
        )
        self.slider.set(0)
        self.slider.pack(side="left", padx=10)
        
        # Title label
        title_label = ctk.CTkLabel(
            self,
            text="THROTTLE POSITION",
            font=("Arial", 20, "bold"),
            text_color="white"
        )
        title_label.pack()
        
        # Value display frame (to center both value and %)
        value_frame = ctk.CTkFrame(content_frame, fg_color="transparent")
        value_frame.pack(side="left", padx=10)
        
        # Digital display - value and % in same row
        self.value_display = ctk.CTkFrame(value_frame, fg_color="transparent")
        self.value_display.pack()
        
        self.value_label = ctk.CTkLabel(
            self.value_display,
            text="0",
            font=("Arial", 28, "bold"),
            text_color="#FFA500"
        )
        self.value_label.pack(side="left")
        
        self.percent_label = ctk.CTkLabel(
            self.value_display,
            text="%",
            font=("Arial", 28, "bold"),
            text_color="#FFA500"
        )
        self.percent_label.pack(side="left")
        
    def adjust_throttle(self, change):
        current = int(self.slider.get())
        new_value = max(0, min(100, current + change))
        self.slider.set(new_value)
        self.update_display(new_value)

    def on_slide(self, value):
        snapped_value = round(float(value))
        if snapped_value != self.slider.get():
            self.slider.set(snapped_value)
        self.update_display(snapped_value)

    def update_display(self, value):
        self.value_label.configure(text=f"{int(value)}")
        print(f"Throttle position: {int(value)}%")      
      
class MassAirFlowMeter(ctk.CTkFrame):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self.configure(fg_color="transparent")
        
        # Create MAF meter (0-655 g/sec)
        self.meter = Meter(
            self,
            radius=250,
            start=0,
            end=655,
            border_width=4,
            fg="black",
            bg="#1a1a1a",
            text_color="white",
            start_angle=220,
            end_angle=-320,
            text_font=("Arial", 16),
            scale_color="white",
            needle_color="#FF8C00",  # Dark orange color
            scroll_steps=65  # 65 g/sec increments
        )
        self.current_value = 0
        self.meter.set(self.current_value)
        self.meter.pack(pady=15)
        
        # Title label
        title_label = ctk.CTkLabel(
            self,
            text="MASS AIR FLOW",
            font=("Arial", 20, "bold"),
            text_color="white"
        )
        title_label.pack(pady=(0, 5))
        
        # Digital display        
        self.value_label = ctk.CTkLabel(
            self,
            text=f"{self.current_value} g/sec",
            font=("Arial", 28, "bold"),
            text_color="#FF8C00"  # Matches needle color
        )
        self.value_label.pack(pady=(0, 15))
        
        self.meter.bind("<ButtonRelease-1>", lambda e: self.on_change())
        self.meter.bind("<B1-Motion>", lambda e: self.on_change())

    def increase(self):
        new_value = min(655, self.current_value + 65)  # 65 g/sec increment
        self.set_value(new_value)

    def decrease(self):
        new_value = max(0, self.current_value - 65)  # 65 g/sec decrement
        self.set_value(new_value)

    def set_value(self, value):
        self.current_value = value
        self.meter.set(self.current_value)
        self.value_label.configure(text=f"{self.current_value} g/sec")
        print(f"Mass air flow: {self.current_value} g/sec")

    def on_change(self):
        current = self.meter.get()
        rounded = round(current / 65) * 65  # Round to nearest 65
        if rounded != self.current_value:
            self.set_value(rounded)