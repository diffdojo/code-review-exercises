class Workshop < ApplicationRecord
  has_many :signups, dependent: :destroy

  validates :title, :starts_on, :signup_closes_on, presence: true

  def signup_open?
    Date.current < signup_closes_on
  end
end
