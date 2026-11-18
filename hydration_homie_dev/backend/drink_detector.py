
def classify_event(
    bottle_lifted,
    max_tilt_degrees,
    tilt_duration_seconds,
    weight_before_g,
    weight_after_g,
    refill_mode=False
):
    """
    Classify a completed bottle activity.

    Assumes weights are calibrated and stable.
    """

    weight_difference = weight_before_g - weight_after_g

    # 1. Refill button was activated
    if refill_mode:
        amount_added = max(0, -weight_difference)

        if amount_added > 0:
            return {
                "event": "refill",
                "amount_ml": round(amount_added, 1)
            }

        return {
            "event": "ignored",
            "amount_ml": 0
        }

    # 2. Drinking detection
    if (
        bottle_lifted
        and max_tilt_degrees >= 45
        and tilt_duration_seconds >= 1.0
        and 10 <= weight_difference <= 500
    ):
        return {
            "event": "drink",
            "amount_ml": round(weight_difference, 1)
        }

    # 3. Possible refill without button
    if weight_difference < -10:
        return {
            "event": "suspected_refill",
            "amount_ml": round(-weight_difference, 1)
        }

    # 4. Weight decreased without drinking motion
    if weight_difference >= 10:
        return {
            "event": "uncertain",
            "amount_ml": round(weight_difference, 1)
        }

    # 5. No meaningful change
    return {
        "event": "ignored",
        "amount_ml": 0
    }
