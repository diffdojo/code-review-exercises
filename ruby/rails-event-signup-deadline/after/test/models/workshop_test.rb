require "test_helper"

class WorkshopTest < ActiveSupport::TestCase
  setup do
    @workshop = Workshop.new(
      title: "Intro to pottery",
      starts_on: Date.new(2026, 11, 21),
      signup_closes_on: Date.new(2026, 11, 14)
    )
  end

  test "sign-up is open a few days before it closes" do
    travel_to Time.zone.local(2026, 11, 10, 18, 0) do
      assert @workshop.signup_open?
    end
  end

  test "sign-up is closed once the closing date has passed" do
    travel_to Time.zone.local(2026, 11, 16, 9, 0) do
      assert_not @workshop.signup_open?
    end
  end
end
