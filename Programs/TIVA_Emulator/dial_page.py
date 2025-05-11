import customtkinter as ctk
from base_page import BasePage
from meters import (EngineSpeedMeter, VehicleSpeedMeter, 
                   PressureGauge, ThrottlePositionSlider,
                   MassAirFlowMeter)

class DialPage(BasePage):
    def __init__(self, parent, controller, can_processor, serial_handler):
        super().__init__(parent, controller)
        self.can_processor = can_processor
        self.serial_handler = serial_handler
        self.displayed_frames = []  # Track displayed frames to avoid duplicates
        
        # Main frame with minimal padding
        control_frame = ctk.CTkFrame(self.middle_frame, fg_color="transparent")
        control_frame.pack(expand=True, fill="both", padx=20, pady=10)
        
        # Title
        ctk.CTkLabel(
            control_frame,
            text="Sensor Data - Parameter IDentifiers", 
            font=("Arial", 24, "bold")
        ).pack(pady=(0, 15))
        
        # Meters frame using grid for tight layout
        meters_frame = ctk.CTkFrame(control_frame, fg_color="transparent")
        meters_frame.pack(expand=True, fill="both")
        
        # Configure equal columns (now 5 columns)
        for i in range(5):
            meters_frame.grid_columnconfigure(i, weight=1)
        
        # Create all meter widgets first
        self._create_engine_meter(meters_frame)
        self._create_speed_meter(meters_frame)
        self._create_pressure_gauge(meters_frame)
        self._create_throttle_slider(meters_frame)
        self._create_maf_meter(meters_frame)
        
        # CAN Frame Display Section
        self._create_can_frame_display(control_frame)
        
        # Set meter references in CAN processor
        self._setup_can_processor()
        
        # Start CAN message update loop
        self.after(100, self._update_can_display)

    def _create_can_frame_display(self, parent_frame):
        """Create the CAN frame display area"""
        # Frame for CAN messages
        can_frame = ctk.CTkFrame(parent_frame)
        can_frame.pack(fill="x", pady=(20, 0))
        
        # Title
        ctk.CTkLabel(
            can_frame,
            text="CAN COMMUNICATION - LIVE DATA STREAM",
            font=("Arial", 16, "bold")
        ).pack(pady=(5, 10))
        
        # Main display frame for CAN messages
        self.can_display_frame = ctk.CTkFrame(can_frame, height=200)
        self.can_display_frame.pack(fill="both", expand=True, padx=5, pady=(0, 10))
        
        # Initialize display labels
        self.can_labels = []
        for i in range(3):  # Show up to 3 frame pairs
            frame = ctk.CTkFrame(self.can_display_frame)
            frame.pack(fill="x", pady=2)
            
            label = ctk.CTkLabel(
                frame,
                text="Waiting for CAN frames...",
                font=("Consolas", 14),
                anchor="w",
                justify="left",
                height=40
            )
            label.pack(fill="x")
            self.can_labels.append(label)
        
        # Control buttons
        btn_frame = ctk.CTkFrame(can_frame, fg_color="transparent")
        btn_frame.pack(fill="x", pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Refresh Display",
            command=self._refresh_can_display,
            width=150
        ).pack(side="left", padx=5)

    def _update_can_display(self):
        """Update the CAN message display with new messages"""
        messages = self.can_processor.get_messages()
        new_messages = [msg for msg in messages if msg not in self.displayed_frames]
        
        if new_messages:
            # Keep only the last 3 unique frame pairs (request + response)
            self.displayed_frames = messages[-6:]
            
            # Update the display labels
            for i, msg in enumerate(self.displayed_frames[-3:]):
                frame_type = "REQUEST" if "07 E8" not in msg else "RESPONSE"
                color = "#FF6347" if "07 E8" not in msg else "#7CFC00"
                
                self.can_labels[i].configure(
                    text=f"{frame_type}: {msg}",
                    text_color=color
                )
        
        # Schedule next update
        self.after(200, self._update_can_display)

    def _refresh_can_display(self):
        """Clear and refresh the display"""
        self.displayed_frames = []
        for label in self.can_labels:
            label.configure(text="Waiting for CAN frames...", text_color="white")

    def _create_engine_meter(self, parent_frame):
        """Create engine RPM meter with controls"""
        frame = ctk.CTkFrame(parent_frame, fg_color="transparent")
        frame.grid(row=0, column=0, sticky="nsew", padx=5)
        
        self.engine_meter = EngineSpeedMeter(frame)
        self.engine_meter.pack(pady=(0, 10))
        
        btn_frame = ctk.CTkFrame(frame, fg_color="transparent")
        btn_frame.pack()
        
        ctk.CTkButton(
            btn_frame,
            text="Decrease (-1000)",
            command=lambda: self._adjust_meter(self.engine_meter.decrease),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack(pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Increase (+1000)",
            command=lambda: self._adjust_meter(self.engine_meter.increase),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack()

    def _create_speed_meter(self, parent_frame):
        """Create vehicle speed meter with controls"""
        frame = ctk.CTkFrame(parent_frame, fg_color="transparent")
        frame.grid(row=0, column=1, sticky="nsew", padx=5)
        
        self.speed_meter = VehicleSpeedMeter(frame)
        self.speed_meter.pack(pady=(0, 10))
        
        btn_frame = ctk.CTkFrame(frame, fg_color="transparent")
        btn_frame.pack()
        
        ctk.CTkButton(
            btn_frame,
            text="Decrease (-5)",
            command=lambda: self._adjust_meter(self.speed_meter.decrease),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack(pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Increase (+5)",
            command=lambda: self._adjust_meter(self.speed_meter.increase),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack()

    def _create_pressure_gauge(self, parent_frame):
        """Create fuel pressure gauge with controls"""
        frame = ctk.CTkFrame(parent_frame, fg_color="transparent")
        frame.grid(row=0, column=2, sticky="nsew", padx=5)
        
        self.pressure_gauge = PressureGauge(frame)
        self.pressure_gauge.pack(pady=(0, 10))
        
        btn_frame = ctk.CTkFrame(frame, fg_color="transparent")
        btn_frame.pack()
        
        ctk.CTkButton(
            btn_frame,
            text="Decrease (-50)",
            command=lambda: self._adjust_meter(self.pressure_gauge.decrease),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack(pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Increase (+50)",
            command=lambda: self._adjust_meter(self.pressure_gauge.increase),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack()

    def _create_throttle_slider(self, parent_frame):
        """Create throttle position slider with controls"""
        frame = ctk.CTkFrame(parent_frame, fg_color="transparent")
        frame.grid(row=0, column=3, sticky="nsew", padx=5)
        
        self.throttle_slider = ThrottlePositionSlider(frame)
        self.throttle_slider.pack(pady=(0, 10))
        
        btn_frame = ctk.CTkFrame(frame, fg_color="transparent")
        btn_frame.pack()
        
        ctk.CTkButton(
            btn_frame,
            text="Decrease (-10%)",
            command=lambda: self._adjust_meter(lambda: self.throttle_slider.adjust_throttle(-10)),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack(pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Increase (+10%)",
            command=lambda: self._adjust_meter(lambda: self.throttle_slider.adjust_throttle(10)),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack()

    def _create_maf_meter(self, parent_frame):
        """Create MAF meter with controls"""
        frame = ctk.CTkFrame(parent_frame, fg_color="transparent")
        frame.grid(row=0, column=4, sticky="nsew", padx=5)
        
        self.maf_meter = MassAirFlowMeter(frame)
        self.maf_meter.pack(pady=(0, 10))
        
        btn_frame = ctk.CTkFrame(frame, fg_color="transparent")
        btn_frame.pack()
        
        ctk.CTkButton(
            btn_frame,
            text="Decrease (-65)",
            command=lambda: self._adjust_meter(self.maf_meter.decrease),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack(pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Increase (+65)",
            command=lambda: self._adjust_meter(self.maf_meter.increase),
            width=140,
            height=30,
            font=("Arial", 12)
        ).pack()

    def _setup_can_processor(self):
        """Initialize CAN processor with meter references"""
        self.can_processor.set_meters(
            engine_meter=self.engine_meter,
            speed_meter=self.speed_meter,
            pressure_gauge=self.pressure_gauge,
            throttle_slider=self.throttle_slider,
            maf_meter=self.maf_meter
        )

    def _adjust_meter(self, adjust_func):
        """Wrapper for meter adjustment functions"""
        adjust_func()