# Show the assignee on the support ticket card

React · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- The support queue renders a TicketCard for every ticket: subject, a priority label, an assignee line, and the ticket number with the open date.
- ticket.assignee is either an object with a non-empty string name, or null. New tickets start unassigned, so assignee is null until an agent picks the ticket up — a routine state, not an error.
- Unassigned tickets must render the assignee line as 'Assigned to: Unassigned'.
- ticket.priority is exactly 'high' or 'normal'; subject, id and openedAt are always present.

## Pull request description

Adds the assignee line to the support ticket card, as the triage team asked. Falls back to 'Unassigned' when there is no assignee name. Clicked through the queue locally and every card shows the right agent.

## Context

The card renders across the shared support queue, where at any moment a chunk of tickets are freshly created and still unassigned.

## Diff

`src/components/TicketCard.jsx` (modified)

```diff
--- a/src/components/TicketCard.jsx
+++ b/src/components/TicketCard.jsx
@@ -5,12 +5,14 @@
 }
 
 export default function TicketCard({ ticket }) {
+  const assigneeName = ticket.assignee.name || 'Unassigned';
   return (
     <article className="ticket-card">
       <header>
         <h3>{ticket.subject}</h3>
         <span className="priority">{priorityLabel(ticket.priority)}</span>
       </header>
+      <p className="assignee">Assigned to: {assigneeName}</p>
       <footer>#{ticket.id} · opened {ticket.openedAt}</footer>
     </article>
   );
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Show the assignee on the support ticket card](https://app.diffdojo.com/challenges/ticket-card-assignee?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.

A written walkthrough of this PR, answer included: https://diffdojo.com/blog/how-to-stop-rubber-stamping-ai-code
