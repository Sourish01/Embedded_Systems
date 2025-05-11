import customtkinter as ctk

class BasePage(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        self.controller = controller

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        self.middle_frame = ctk.CTkFrame(self)
        self.middle_frame.grid(row=0, column=0, sticky="nsew", padx=20, pady=10)

        self.bottom_frame = ctk.CTkFrame(self, height=100)
        self.bottom_frame.grid(row=1, column=0, sticky="sew", padx=20, pady=(10, 20))

        self.create_navigation()

    def create_navigation(self):
        nav_frame = ctk.CTkFrame(self.bottom_frame, fg_color="transparent")
        nav_frame.pack(pady=10)

        current_page = self.__class__.__name__

        if current_page != "HomePage":
            home_btn = ctk.CTkButton(
                nav_frame,
                text="Home",
                command=lambda: self.controller.show_page("HomePage"),
                width=160,
                height=50,
                font=ctk.CTkFont(size=16)
            )
            home_btn.pack(side="left", padx=20)

        if current_page != "DialPage":
            dash_btn = ctk.CTkButton(
                nav_frame,
                text="Dashboard",
                command=lambda: self.controller.show_page("DialPage"),
                width=160,
                height=50,
                font=ctk.CTkFont(size=16)
            )
            dash_btn.pack(side="left", padx=20)

        if current_page != "FaultPage":
            fault_btn = ctk.CTkButton(
                nav_frame,
                text="Faults",
                command=lambda: self.controller.show_page("FaultPage"),
                width=160,
                height=50,
                font=ctk.CTkFont(size=16)
            )
            fault_btn.pack(side="left", padx=20)