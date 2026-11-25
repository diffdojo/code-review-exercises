# Add-to-cart helper

Python · FastAPI · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- add_item(name, cart=...) appends an item and returns the cart.
- Called with no cart, it must start from an empty cart.
- Two independent calls with no cart must not share state.

## Pull request description

Adds the cart helper: appends the item and returns the cart, defaulting to a fresh empty cart when none is passed. Verified by adding a couple of items.

## Diff

`app/cart.py` (new file)

```diff
--- /dev/null
+++ b/app/cart.py
@@ -0,0 +1,3 @@
+def add_item(name: str, cart: list[str] = []) -> list[str]:
+    cart.append(name)
+    return cart
```

The full files are in [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Add-to-cart helper](https://app.diffdojo.com/challenges/add-to-cart-helper?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.

A written walkthrough of this PR, answer included: https://diffdojo.com/blog/how-to-keep-code-reading-skills-sharp
