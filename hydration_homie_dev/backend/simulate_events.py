
import uuid
import time
import requests

from drink_detector import classify_event

API_URL = "http://127.0.0.1:8000/api/events"

DEVICE_ID = "hydration-homie-01"

# Simulated sensor scenarios
scenarios = [
    {
        "name": "Normal drinking",
        "bottle_lifted": True,
        "max_tilt_degrees": 65,
        "tilt_duration_seconds": 2.5,
        "weight_before_g": 650,
        "weight_after_g": 530,
        "refill_mode": False
    },
    {
        "name": "Small sip",
        "bottle_lifted": True,
        "max_tilt_degrees": 55,
        "tilt_duration_seconds": 1.5,
        "weight_before_g": 530,
        "weight_after_g": 505,
        "refill_mode": False
    },
    {
        "name": "Carrying bottle",
        "bottle_lifted": True,
        "max_tilt_degrees": 10,
        "tilt_duration_seconds": 0,
        "weight_before_g": 505,
        "weight_after_g": 505,
        "refill_mode": False
    },
    {
        "name": "Refilling bottle",
        "bottle_lifted": True,
        "max_tilt_degrees": 30,
        "tilt_duration_seconds": 0,
        "weight_before_g": 505,
        "weight_after_g": 905,
        "refill_mode": True
    },
    {
        "name": "Drinking after refill",
        "bottle_lifted": True,
        "max_tilt_degrees": 70,
        "tilt_duration_seconds": 3.0,
        "weight_before_g": 905,
        "weight_after_g": 755,
        "refill_mode": False
    },
    {
        "name": "Bottle shaken",
        "bottle_lifted": True,
        "max_tilt_degrees": 25,
        "tilt_duration_seconds": 0.5,
        "weight_before_g": 755,
        "weight_after_g": 755,
        "refill_mode": False
    }
]

def run_simulation():
    print("\nHYDRATION HOMIE SIMULATION")
    print("=" * 45)

    for scenario in scenarios:
        print(f"\nScenario: {scenario['name']}")

        result = classify_event(
            bottle_lifted=scenario["bottle_lifted"],
            max_tilt_degrees=scenario["max_tilt_degrees"],
            tilt_duration_seconds=scenario["tilt_duration_seconds"],
            weight_before_g=scenario["weight_before_g"],
            weight_after_g=scenario["weight_after_g"],
            refill_mode=scenario["refill_mode"]
        )

        print("Classification:", result["event"])
        print("Amount:", result["amount_ml"], "mL")

        if result["event"] in ("drink", "refill"):
            payload = {
                "event_id": str(uuid.uuid4()),
                "device_id": DEVICE_ID,
                "event_type": result["event"],
                "amount_ml": result["amount_ml"]
            }

            try:
                response = requests.post(
                    API_URL,
                    json=payload,
                    timeout=5
                )
                response.raise_for_status()

                print("Sent to FastAPI!")
                print("Response:", response.json())

            except requests.RequestException as error:
                print("API request failed:", error)

        else:
            print("Not sent to FastAPI.")

        time.sleep(1)

    print("\nSimulation complete!")


if __name__ == "__main__":
    run_simulation()
