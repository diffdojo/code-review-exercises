package export

import (
	"encoding/csv"
	"io"
	"strconv"
	"time"
)

// WritePayoutsCSV writes the payouts statement that accountants download.
func WritePayoutsCSV(w io.Writer, payouts []Payout) error {
	cw := csv.NewWriter(w)
	if err := cw.Write([]string{"paid_at", "currency", "amount_minor", "reference"}); err != nil {
		return err
	}
	for _, p := range payouts {
		rec := []string{
			p.PaidAt.UTC().Format(time.DateOnly),
			p.Currency,
			strconv.FormatInt(p.AmountMinor, 10),
			p.Reference,
		}
		if err := cw.Write(rec); err != nil {
			return err
		}
	}
	cw.Flush()
	return cw.Error()
}
