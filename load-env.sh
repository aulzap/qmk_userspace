# Set QMK config for bastardkb-qmk repo
cd /home/arnt/gh/bastardkb-qmk
qmk config user.qmk_home (realpath .)
qmk config user.overlay_dir (realpath .)

# Set QMK config for bastard_kb_qmk_userspace repo (this script's location)
cd /home/arnt/gh/bastard_kb_qmk_userspace
qmk config user.qmk_home (realpath .)
qmk config user.overlay_dir (realpath .)

cd /home/arnt/gh/bastard_kb_qmk_userspace