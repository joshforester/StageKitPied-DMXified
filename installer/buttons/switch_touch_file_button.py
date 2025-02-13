[Unit]
Description=File Touching Button Monitor Service
After=network.target

[Service]
ExecStart=/usr/bin/python3 /opt/StageKitPied/switch_run_script_button.py 17 /opt/StageKitPied/kill_skp.sh /opt/StageKitPied/run_skp.sh
Restart=always
Environment=PYTHONUNBUFFERED=1

[Install]
WantedBy=multi-user.target

