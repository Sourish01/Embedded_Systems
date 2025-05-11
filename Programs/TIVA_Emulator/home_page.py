import customtkinter as ctk
from base_page import BasePage
import os
from PIL import Image, ImageTk


class HomePage(BasePage):
    def __init__(self, parent, controller):
        super().__init__(parent, controller)
        
        # Configure middle frame
        self.middle_frame.grid_rowconfigure(0, weight=1)
        self.middle_frame.grid_columnconfigure(0, weight=1)
        
        # Content frame
        welcome_frame = ctk.CTkFrame(self.middle_frame)
        welcome_frame.grid(row=0, column=0, sticky="nsew", padx=20, pady=20)
        
        # Welcome message
        welcome_label = ctk.CTkLabel(
            welcome_frame, 
            text="Car Emulator", 
            font=("Segoe UI", 24, "bold")
        )
        welcome_label.pack(pady=(40, 20))
        
        # Description
        desc_label = ctk.CTkLabel(
            welcome_frame, 
            text="A User interactive application for Functional Emulation of Car",
            font=("Segoe UI", 14)
        )
        desc_label.pack(pady=(0, 30))
        
        # Initialize logo label (larger size)
        self.logo_label = ctk.CTkLabel(
            self,
            text="",  # Start with empty text
            fg_color="transparent"
        )
        self.load_logo()  # Load the logo
        
    def load_logo(self):
        """Load and position the DESE logo from Desktop"""
        try:
            # Get the desktop path
            desktop_path = os.path.join(os.path.expanduser('~'), 'Desktop')
            logo_filename = "DESE_image.png"
            logo_path = os.path.join(desktop_path, logo_filename)
            
            print(f"Attempting to load logo from: {logo_path}")
            
            # Open and resize the logo (larger size - 250x100 instead of 150x50)
            logo_image = Image.open(logo_path)
            logo_image = logo_image.resize((250, 100), Image.LANCZOS)
            self.logo_photo = ImageTk.PhotoImage(logo_image)
            
            # Update the label with the logo
            self.logo_label.configure(
                image=self.logo_photo,
                text=""  # Clear any fallback text
            )
            
        except Exception as e:
            print(f"Error loading logo: {e}")
            # Fallback to text if image fails
            self.logo_label.configure(
                image=None,
                text="DESE",
                font=("Arial", 20, "bold"),  # Larger font size
                text_color="white"
            )
        
        # Position the logo in top-right corner with padding
        self.logo_label.place(relx=1.0, rely=0.0, anchor='ne', x=-20, y=20)