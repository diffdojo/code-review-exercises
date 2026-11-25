# Code review exercises

10 small pull requests to practise code review on, one per stack. Each has a
ticket, a PR description and a diff. Your job is the reviewer's: read it and
decide whether to approve or request changes, and say why.

The PRs look like what an AI coding assistant produces on an ordinary day:
tidy, plausible, with a confident description. Some of them do not do what
the ticket asks.

## The exercises

| Stack | Pull request | Level | Files |
|---|---|---|---|
| Python | [Add-to-cart helper](python/add-to-cart-helper) | beginner | 1 |
| React | [Show the assignee on the support ticket card](react/ticket-card-assignee) | beginner | 1 |
| Angular | [Show customer emails with their formatting in the ticket view](angular/angular-ticket-email-body) | beginner | 1 |
| Node | [Bulk import users](node/bulk-user-import) | beginner | 2 |
| Java | [Skip repeated addresses in contact imports](java/spring-contact-import-dedupe) | beginner | 2 |
| Go | [Payouts CSV: amounts in major units](go/go-payouts-csv-major-units) | beginner | 1 |
| C# | [Newest members card on the workspace home page](csharp/aspnet-newest-members) | beginner | 2 |
| C++ | [High score table for the arcade cabinet](cpp/cpp-leaderboard-print) | beginner | 3 |
| Ruby | [Close workshop sign-up after the closing date](ruby/rails-event-signup-deadline) | beginner | 3 |
| Database | [Database login for the support team's Metabase](sql/pg-support-readonly-role) | beginner | 1 |

## How to use them

1. Open an exercise and read the ticket before the diff.
2. Write down what would make you reject the change.
3. Read the diff. The full files are in `before/` and `after/` if you want to
   run or test them.
4. Write your verdict and the comment you would leave, with the line.
5. Check yourself with the link at the bottom of the exercise.

There are no answers in this repository. Each exercise links to the same PR
in [DiffDojo](https://app.diffdojo.com/?utm_source=github&utm_campaign=exercises), where you comment on the lines, submit a verdict
and get the expected review next to yours. These 10 are open there
without an account.

Good for: getting ready for a code review interview round, a 15-minute team
session where everyone reviews the same PR, or a habit of reading code that
you did not write.

## More

DiffDojo has 430+ of these across the same stacks, including PRs that are
correct and should be approved. A free account opens one more PR of your
choice; the rest of the library is a pass (https://diffdojo.com/pricing/).

Related reading:

- [How to practice code review](https://diffdojo.com/blog/how-to-practice-code-review)
- [How to review AI-generated code](https://diffdojo.com/blog/how-to-review-ai-generated-code)
- [The code review interview](https://diffdojo.com/blog/code-review-interview)

## Questions and mistakes

Found an error in an exercise, or disagree with how one is framed? Open an
issue. Please do not post the answer to an exercise in an issue title.

Pull requests with new exercises are not accepted: the files here are
generated from the DiffDojo library, so a change made here would be
overwritten.

## License

The exercises (text and code) are © DiffDojo and licensed under
[CC BY-NC-ND 4.0](LICENSE). You can read them, share them and link to them
with attribution. You may also use them unchanged for practice inside your
own team. You may not sell them, change them or republish them inside another
product or course.
