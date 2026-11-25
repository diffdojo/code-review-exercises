# Close workshop sign-up after the closing date

Ruby · Rails · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- Each workshop has a `signup_closes_on` date, which the workshop page already advertises as "Sign-up open until <date>". Sign-up is open through that date, inclusive: members can sign up on every day up to and including `signup_closes_on`, and sign-up closes when that day ends. From the day after, sign-up is closed.
- "Today" is the current date in the app's time zone (`config.time_zone = "Europe/Berlin"`); `signup_closes_on` is a calendar date in that same zone.
- `POST /workshops/:workshop_id/signup` (existing) signs the current user up. While sign-up is closed it answers 422 with `{"error": "sign-up closed"}` and creates no sign-up; the closed check comes before the duplicate check, so a member who is already signed up also gets this 422 once sign-up has closed.
- Unchanged: a signed-out request answers 401; an unknown workshop answers 404; while sign-up is open, a successful sign-up answers 201 with `workshop_id` and `signed_up_at` and signing up twice for the same workshop answers 409 `{"error": "already signed up"}`.

## Pull request description

Enforces the sign-up closing date that the workshop page already shows. `Workshop#signup_open?` (new, `app/models/workshop.rb`) compares today's date in the app's time zone with `signup_closes_on`. `SignupsController#create` (`app/controllers/signups_controller.rb`) checks it before creating the sign-up and answers 422 `sign-up closed` when sign-up is over (also for a member already signed up). The 401 and 404 paths are unchanged, and while sign-up is open so are the 201 and 409 paths. `test/models/workshop_test.rb` (new) adds Minitest model tests for `signup_open?` using `travel_to`.

## Context

Ruby 4.0, Rails 8.1 JSON API (`ApplicationController < ActionController::API`), SQLite in development and test, PostgreSQL 16 in production; nothing in this change depends on the database. Existing and unchanged: `config.time_zone = "Europe/Berlin"`, so `Date.current` is today's date in Berlin; `ApplicationController` includes the app's `Authentication` concern, whose `before_action :require_authentication` answers 401 when signed out and otherwise sets `Current.user`; `config/routes.rb` nests `resource :signup, only: :create` under `resources :workshops, only: []`. Every signed-in member may sign up for any workshop. The `workshops` table has `title`, `starts_on` and `signup_closes_on`, all `null: false`, and `signup_closes_on` is a `date` column set by the organizer form (unchanged). `signups` has a unique index on `[workshop_id, user_id]`; `Signup` belongs to `workshop` and `user`. The workshop page (shipped separately) shows the closing date and does not change here.

## Diff

`app/models/workshop.rb` (modified)

```diff
--- a/app/models/workshop.rb
+++ b/app/models/workshop.rb
@@ -2,4 +2,8 @@
   has_many :signups, dependent: :destroy
 
   validates :title, :starts_on, :signup_closes_on, presence: true
+
+  def signup_open?
+    Date.current < signup_closes_on
+  end
 end
```

`app/controllers/signups_controller.rb` (modified)

```diff
--- a/app/controllers/signups_controller.rb
+++ b/app/controllers/signups_controller.rb
@@ -1,6 +1,11 @@
 class SignupsController < ApplicationController
   def create
     workshop = Workshop.find(params[:workshop_id])
+    unless workshop.signup_open?
+      render json: { error: "sign-up closed" }, status: :unprocessable_content
+      return
+    end
+
     signup = workshop.signups.create!(user: Current.user)
     render json: { workshop_id: workshop.id, signed_up_at: signup.created_at }, status: :created
   rescue ActiveRecord::RecordNotUnique
```

`test/models/workshop_test.rb` (new file)

```diff
--- /dev/null
+++ b/test/models/workshop_test.rb
@@ -0,0 +1,23 @@
+require "test_helper"
+
+class WorkshopTest < ActiveSupport::TestCase
+  setup do
+    @workshop = Workshop.new(
+      title: "Intro to pottery",
+      starts_on: Date.new(2026, 11, 21),
+      signup_closes_on: Date.new(2026, 11, 14)
+    )
+  end
+
+  test "sign-up is open a few days before it closes" do
+    travel_to Time.zone.local(2026, 11, 10, 18, 0) do
+      assert @workshop.signup_open?
+    end
+  end
+
+  test "sign-up is closed once the closing date has passed" do
+    travel_to Time.zone.local(2026, 11, 16, 9, 0) do
+      assert_not @workshop.signup_open?
+    end
+  end
+end
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Close workshop sign-up after the closing date](https://app.diffdojo.com/challenges/rails-event-signup-deadline?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
