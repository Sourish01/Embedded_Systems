DTC_DATABASE = {
    "P0171": {
        "description": "System Too Lean (Bank 1)",
        "severity": "high",
        "bytes": ["0x01", "0x71"]
    },
    "P0172": {
        "description": "System Too Rich (Bank 1)",
        "severity": "high",
        "bytes": ["0x01", "0x72"]
    },
    "P0300": {
        "description": "Random/Multiple Cylinder Misfire",
        "severity": "critical",
        "bytes": ["0x03", "0x00"]
    },
    "P0301": {
        "description": "Cylinder 1 Misfire Detected",
        "severity": "critical",
        "bytes": ["0x03", "0x01"]
    },
    "P0128": {
        "description": "Coolant Thermostat Malfunction",
        "severity": "medium",
        "bytes": ["0x01", "0x28"]
    },
    "P0420": {
        "description": "Catalyst Efficiency Below Threshold (Bank 1)",
        "severity": "medium",
        "bytes": ["0x04", "0x20"]
    },
    "P0135": {
        "description": "O2 Sensor Heater Circuit (Bank 1 Sensor 1)",
        "severity": "medium",
        "bytes": ["0x01", "0x35"]
    },
    "P0700": {
        "description": "Transmission Control System Malfunction",
        "severity": "high",
        "bytes": ["0x07", "0x00"]
    },
    "P0562": {
        "description": "System Voltage Low",
        "severity": "high",
        "bytes": ["0x05", "0x62"]
    },
    "P0299": {
        "description": "Turbo Underboost Condition",
        "severity": "high",
        "bytes": ["0x02", "0x99"]
    }
}

SEVERITY_COLORS = {
    "low": "green",
    "medium": "orange",
    "high": "red",
    "critical": "darkred"
}

DARKER_COLORS = {
    "info": "#2980b9",
    "low": "#27ae60",
    "medium": "#d35400",
    "high": "#c0392b",
    "critical": "#7b241c"
}