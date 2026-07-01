import os
import json
from datetime import datetime, timedelta

def create_mock_data():
    base_dir = os.getcwd()
    unusual_dir = os.path.join(base_dir, "unusual")
    video_dir = os.path.join(base_dir, "video", "unusual")

    os.makedirs(unusual_dir, exist_ok=True)
    os.makedirs(video_dir, exist_ok=True)

    events = []

    event_templates = [
        {"type": "画面遮挡", "level": "紧急", "status": "待处理"},
        {"type": "人脸识别异常", "level": "警告", "status": "处理中"},
        {"type": "画面遮挡", "level": "紧急", "status": "已处理"},
        {"type": "人脸识别异常", "level": "警告", "status": "待处理"},
        {"type": "画面遮挡", "level": "通知", "status": "待处理"}
    ]

    now = datetime.now()

    for i, tpl in enumerate(event_templates):
        timestamp_str = (now - timedelta(minutes=i*10)).strftime("%Y%m%d-%H%M%S")
        formatted_time = (now - timedelta(minutes=i*10)).strftime("%Y-%m-%d %H:%M:%S")

        video_name = f"mock_event_{timestamp_str}.avi"
        thumb_name = f"mock_event_{timestamp_str}_thumb.jpg"

        video_path = os.path.join(video_dir, video_name)
        thumb_path = os.path.join(video_dir, thumb_name)

        # Create dummy files
        with open(video_path, 'w') as f:
            f.write('')
        with open(thumb_path, 'w') as f:
            f.write('')

        event = {
            "screenshot": thumb_path,
            "type": tpl["type"],
            "level": tpl["level"],
            "time": formatted_time,
            "status": tpl["status"],
            "videoPath": video_path
        }
        events.append(event)

    json_path = os.path.join(unusual_dir, "unusual_events.json")
    with open(json_path, 'w', encoding='utf-8') as f:
        json.dump(events, f, ensure_ascii=False, indent=4)

    print(f"Mock data generated at {json_path}")

if __name__ == "__main__":
    create_mock_data()
