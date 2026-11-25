package export

import (
	"encoding/csv"
	"fmt"
	"io"
	"time"
)

// WritePayoutsCSV writes the payouts statement that accountants download.
func WritePayoutsCSV(w io.Writer, payouts []Payout) error {
	cw := csv.NewWriter(w)
	if err := cw.Write([]string{"paid_at", "currency", "amount", "reference"}); err != nil {
		return err
	}
	for _, p := range payouts {
		rec := []string{
			p.PaidAt.UTC().Format(time.DateOnly),
			p.Currency,
			formatMajor(p.AmountMinor),
			p.Reference,
		}
		if err := cw.Write(rec); err != nil {
			return err
		}
	}
	cw.Flush()
	return cw.Error()
}

// formatMajor renders an amount held in minor units as a plain decimal in
// major units, the way spreadsheet imports expect it.
func formatMajor(minor int64) string {
	return fmt.Sprintf("%d.%02d", minor/100, minor%100)
}
