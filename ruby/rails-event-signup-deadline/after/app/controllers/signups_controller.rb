class SignupsController < ApplicationController
  def create
    workshop = Workshop.find(params[:workshop_id])
    unless workshop.signup_open?
      render json: { error: "sign-up closed" }, status: :unprocessable_content
      return
    end

    signup = workshop.signups.create!(user: Current.user)
    render json: { workshop_id: workshop.id, signed_up_at: signup.created_at }, status: :created
  rescue ActiveRecord::RecordNotUnique
    render json: { error: "already signed up" }, status: :conflict
  end
end
