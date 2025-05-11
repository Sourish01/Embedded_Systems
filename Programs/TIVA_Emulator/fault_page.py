import customtkinter as ctk
from base_page import BasePage
from dtc_definitions import DTC_DATABASE, SEVERITY_COLORS
from fault_manager import FaultManager

class FaultPage(BasePage):
    def __init__(self, parent, controller, fault_manager, can_processor, serial_handler):
        super().__init__(parent, controller)
        self.fault_manager = fault_manager
        self.can_processor = can_processor
        self.serial_handler = serial_handler
        self.displayed_frames = []
        
        # Configure grid layout
        self.middle_frame.grid_rowconfigure(0, weight=0)  # Header
        self.middle_frame.grid_rowconfigure(1, weight=1)  # Main content
        self.middle_frame.grid_rowconfigure(2, weight=0)  # CAN display
        self.middle_frame.grid_columnconfigure(0, weight=1)
        
        # Header with status indicators
        header_frame = ctk.CTkFrame(self.middle_frame, height=60)
        header_frame.grid(row=0, column=0, sticky="ew", padx=10, pady=5)
        
        ctk.CTkLabel(
            header_frame, 
            text="FAULT DIAGNOSTICS",
            font=("Helvetica", 20, "bold"),
            text_color="#2ecc71"
        ).pack(side="left", padx=20)
        
        status_frame = ctk.CTkFrame(header_frame, fg_color="transparent")
        status_frame.pack(side="right", padx=20)
        
        self.active_count = ctk.CTkLabel(
            status_frame,
            text="Active: 0",
            font=("Helvetica", 14),
            text_color="#e74c3c"
        )
        self.active_count.pack(side="left", padx=10)
        
        self.stored_count = ctk.CTkLabel(
            status_frame,
            text="Stored: 0",
            font=("Helvetica", 14),
            text_color="#f39c12"
        )
        self.stored_count.pack(side="left", padx=10)
        
        # Main content area
        content_frame = ctk.CTkFrame(self.middle_frame)
        content_frame.grid(row=1, column=0, sticky="nsew", padx=10, pady=5)
        content_frame.grid_columnconfigure(0, weight=1)
        content_frame.grid_rowconfigure(1, weight=1)
        
        # DTC injection controls
        input_panel = ctk.CTkFrame(content_frame, height=80)
        input_panel.grid(row=0, column=0, sticky="ew", pady=5)
        
        self.dropdown = ctk.CTkOptionMenu(
            input_panel,
            values=list(DTC_DATABASE.keys()),
            font=("Helvetica", 14),
            width=300,
            dynamic_resizing=True
        )
        self.dropdown.pack(side="left", padx=10, pady=10)
        
        ctk.CTkButton(
            input_panel,
            text="Inject Fault",
            command=self.handle_inject,
            font=("Helvetica", 14),
            fg_color="#3498db",
            width=120
        ).pack(side="left", padx=10)
        
        # DTC display panels
        display_frame = ctk.CTkFrame(content_frame)
        display_frame.grid(row=1, column=0, sticky="nsew", pady=5)
        display_frame.grid_columnconfigure(0, weight=1)
        display_frame.grid_columnconfigure(1, weight=1)
        display_frame.grid_rowconfigure(0, weight=1)
        
        # Active DTCs panel
        active_frame = self._create_dtc_panel(
            display_frame, 
            "ACTIVE DTCs", 
            "#e74c3c",
            row=0, 
            column=0
        )
        self.active_list = self._create_text_widget(active_frame)
        
        # Stored DTCs panel
        stored_frame = self._create_dtc_panel(
            display_frame,
            "STORED DTCs",
            "#f39c12",
            row=0,
            column=1
        )
        self.search_entry = ctk.CTkEntry(
            stored_frame,
            placeholder_text="Search DTCs...",
            width=180,
            font=("Helvetica", 12)
        )
        self.search_entry.pack(side="top", padx=10, pady=5)
        self.search_entry.bind("<KeyRelease>", self.filter_stored_dtcs)
        self.stored_list = self._create_text_widget(stored_frame)
        
        # Control buttons
        control_frame = ctk.CTkFrame(content_frame, height=60)
        control_frame.grid(row=2, column=0, sticky="ew", pady=5)
        
        control_buttons = [
            ("🔄 Refresh", self.update_ui, "#2ecc71"),
            ("➡ Move to Stored", self.move_selected_dtc, "#9b59b6"),
            ("❌ Clear Active", self.clear_active, "#e74c3c"),
            ("🗑️ Clear Stored", self.clear_stored, "#f39c12")
        ]
        
        for text, command, color in control_buttons:
            ctk.CTkButton(
                control_frame,
                text=text,
                command=command,
                font=("Helvetica", 14),
                fg_color=color,
                width=120
            ).pack(side="left", padx=10)
        
        # CAN Frame Display Section
        self._create_can_frame_display()
        
        self.update_ui()
        self.after(100, self._update_can_display)
        
    def _create_can_frame_display(self):
        """Create the CAN frame display area for fault diagnostics"""
        # Frame for CAN messages
        can_frame = ctk.CTkFrame(self.middle_frame)
        can_frame.grid(row=2, column=0, sticky="ew", padx=10, pady=(0, 10))
        
        # Title
        ctk.CTkLabel(
            can_frame,
            text="FAULT DIAGNOSTIC COMMUNICATION",
            font=("Helvetica", 16, "bold")
        ).pack(pady=(5, 10))
        
        # Main display frame for CAN messages
        self.can_display_frame = ctk.CTkFrame(can_frame, height=200)
        self.can_display_frame.pack(fill="both", expand=True, padx=5, pady=(0, 10))
        
        # Initialize display labels for fault frames
        self.fault_labels = []
        for i in range(3):  # Show up to 3 frame pairs
            frame = ctk.CTkFrame(self.can_display_frame)
            frame.pack(fill="x", pady=2)
            
            label = ctk.CTkLabel(
                frame,
                text="Waiting for diagnostic frames...",
                font=("Consolas", 14),
                anchor="w",
                justify="left",
                height=40
            )
            label.pack(fill="x")
            self.fault_labels.append(label)
        
        # Control buttons
        btn_frame = ctk.CTkFrame(can_frame, fg_color="transparent")
        btn_frame.pack(fill="x", pady=(0, 5))
        
        ctk.CTkButton(
            btn_frame,
            text="Refresh Display",
            command=self._refresh_fault_display,
            width=150
        ).pack(side="left", padx=5)

    def _update_can_display(self):
        """Update the fault frame display with new messages"""
        messages = self.can_processor.get_messages()
        fault_messages = [msg for msg in messages if "03" in msg or "04" in msg]
        new_messages = [msg for msg in fault_messages if msg not in self.displayed_frames]
        
        if new_messages:
            # Keep only the last 3 unique fault frames
            self.displayed_frames = fault_messages[-3:]
            
            # Update the display labels
            for i, msg in enumerate(self.displayed_frames[-3:]):
                frame_type = "DTC REQUEST" if "03" in msg else "DTC CLEAR"
                color = "#FFA500" if "03" in msg else "#00BFFF"
                
                self.fault_labels[i].configure(
                    text=f"{frame_type}: {msg}",
                    text_color=color
                )
        
        # Schedule next update
        self.after(200, self._update_can_display)

    def _refresh_fault_display(self):
        """Clear and refresh the fault display"""
        self.displayed_frames = []
        for label in self.fault_labels:
            label.configure(text="Waiting for diagnostic frames...", text_color="white")
    
    def _create_dtc_panel(self, parent, title, color, row, column):
        frame = ctk.CTkFrame(parent)
        frame.grid(row=row, column=column, sticky="nsew", padx=5, pady=5)
        
        header = ctk.CTkFrame(frame, height=40)
        header.pack(fill="x")
        
        ctk.CTkLabel(
            header,
            text=title,
            font=("Helvetica", 16, "bold"),
            text_color=color
        ).pack(side="left", padx=10)
        
        return frame

    def _create_text_widget(self, parent):
        textbox = ctk.CTkTextbox(
            parent,
            font=("Consolas", 14),
            height=300,
            wrap="word",
            activate_scrollbars=True
        )
        textbox.pack(expand=True, fill="both", padx=5, pady=5)
        return textbox

    def move_selected_dtc(self):
        content = self.active_list.get("1.0", "end").strip().splitlines()
        if content:
            last_line = content[-1]
            code = last_line.split(":")[0]
            self.fault_manager.move_to_stored(code)
            self.update_ui()

    def handle_inject(self):
        code = self.dropdown.get()
        self.fault_manager.inject_fault(code)
        self.update_ui()

    def clear_active(self):
        self.fault_manager.clear_active()
        self.update_ui()

    def clear_stored(self):
        self.fault_manager.clear_stored()
        self.update_ui()

    def filter_stored_dtcs(self, event=None):
        search_term = self.search_entry.get().lower()
        self.stored_list.configure(state="normal")
        self.stored_list.delete("1.0", "end")
        
        for code, data in self.fault_manager.stored_dtc.items():
            if search_term in code.lower() or search_term in data['description'].lower():
                color = SEVERITY_COLORS.get(data["severity"], "white")
                self.stored_list.insert("end", f"{code}: {data['description']}\n\n", code)
                self.stored_list.tag_config(code, foreground=color)
        
        self.stored_list.configure(state="disabled")

    def update_counts(self):
        self.active_count.configure(text=f"Active: {len(self.fault_manager.active_dtc)}")
        self.stored_count.configure(text=f"Stored: {len(self.fault_manager.stored_dtc)}")

    def update_ui(self):
        # Update active DTCs
        self.active_list.configure(state="normal")
        self.active_list.delete("1.0", "end")
        for code, data in self.fault_manager.active_dtc.items():
            color = SEVERITY_COLORS.get(data["severity"], "white")
            self.active_list.insert("end", f"{code}: {data['description']}\n", code)
            self.active_list.tag_config(code, foreground=color)
        
        # Update stored DTCs
        self.stored_list.configure(state="normal")
        self.stored_list.delete("1.0", "end")
        for code, data in self.fault_manager.stored_dtc.items():
            color = SEVERITY_COLORS.get(data["severity"], "white")
            self.stored_list.insert("end", f"{code}: {data['description']}\n", code)
            self.stored_list.tag_config(code, foreground=color)
        
        self.active_list.configure(state="disabled")
        self.stored_list.configure(state="disabled")
        self.update_counts()
        self.after(5000, self.update_ui)