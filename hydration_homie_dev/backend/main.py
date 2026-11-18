
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from datetime import datetime, timezone
from typing import Literal
import sqlite3

app = FastAPI(title="Hydration Homie API")

DB_PATH = "hydration.db"

def connect():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

with connect() as conn:
    conn.execute("""
        CREATE TABLE IF NOT EXISTS events (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            event_id TEXT UNIQUE NOT NULL,
            device_id TEXT NOT NULL,
            event_type TEXT NOT NULL,
            amount_ml REAL NOT NULL,
            timestamp TEXT NOT NULL
        )
    """)

class HydrationEvent(BaseModel):
    event_id: str
    device_id: str
    event_type: Literal["drink", "refill"]
    amount_ml: float = Field(gt=0, le=2000)

@app.post("/api/events")
def add_event(event: HydrationEvent):
    timestamp = datetime.now(timezone.utc).isoformat()

    try:
        with connect() as conn:
            conn.execute(
                """
                INSERT INTO events
                (event_id, device_id, event_type,
                 amount_ml, timestamp)
                VALUES (?, ?, ?, ?, ?)
                """,
                (
                    event.event_id,
                    event.device_id,
                    event.event_type,
                    event.amount_ml,
                    timestamp
                )
            )
    except sqlite3.IntegrityError:
        raise HTTPException(
            status_code=409,
            detail="Event ID already exists"
        )

    return {
        "status": "recorded",
        "event_id": event.event_id
    }

@app.get("/api/events")
def get_events():
    with connect() as conn:
        rows = conn.execute(
            "SELECT * FROM events ORDER BY timestamp DESC"
        ).fetchall()

    return [dict(row) for row in rows]

@app.get("/api/summary")
def get_summary():
    today = datetime.now(timezone.utc).date().isoformat()

    with connect() as conn:
        row = conn.execute(
            """
            SELECT COALESCE(SUM(amount_ml), 0)
            FROM events
            WHERE event_type = 'drink'
              AND substr(timestamp, 1, 10) = ?
            """,
            (today,)
        ).fetchone()

    total = row[0]

    return {
        "date": today,
        "total_consumed_ml": total,
        "daily_goal_ml": 2000,
        "progress_percent": round(total / 2000 * 100, 1)
    }
