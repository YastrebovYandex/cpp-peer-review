enum class TimeValidityError {
  YEAR_TOO_SMALL,
  YEAR_TOO_BIG,
  MONTH_TOO_SMALL,
  MONTH_TOO_BIG,
  DAY_TOO_SMALL,
  DAY_TOO_BIG,
  HOUR_TOO_SMALL,
  HOUR_TOO_BIG,
  MINUTES_TOO_BIG,
  MINUTES_TOO_SMALL,
  SECONDS_TOO_BIG,
  SECONDS_TOO_SMALL,
};

std::variant<bool, TimeValidityError> CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
		return TimeValidityError::YEAR_TOO_SMALL;
    }
    if (dt.year > 9999) {
		return TimeValidityError::YEAR_TOO_BIG;
    }

    if (dt.month < 1) {
		return TimeValidityError::MONTH_TOO_SMALL;
    }
    if (dt.month > 12) {
        return TimeValidityError::MONTH_TOO_BIG;
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1) {
		return TimeValidityError::DAY_TOO_SMALL;
    }
    if (dt.day > month_lengths[dt.month - 1]) {
		return TimeValidityError::DAY_TOO_BIG;
    }

    if (dt.hour < 0) {
		return TimeValidityError::HOUR_TOO_SMALL;
    }
    if (dt.hour > 23) {
		return TimeValidityError::HOUR_TOO_BIG;
    }

    if (dt.minute < 0) {
        return TimeValidityError::MINUTES_TOO_SMALL;
    }
    if (dt.minute > 59) {
        return TimeValidityError::MINUTES_TOO_BIG;
    }

    if (dt.second < 0) {
		return TimeValidityError::SECONDS_TOO_SMALL;
    }
    if (dt.second > 59) {
		return TimeValidityError::SECONDS_TOO_BIG;
    }
	
	return true;
}
