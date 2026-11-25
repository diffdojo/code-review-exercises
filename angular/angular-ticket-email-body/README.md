# Show customer emails with their formatting in the ticket view

Angular · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- The ticket page shows each customer message with the formatting it had in the email: paragraphs, line breaks, bold and italic text, lists and links. The message body renders the message's `bodyHtml` instead of the flattened `bodyText`.
- Nothing in a customer's email may run in the agent console: no scripts, no event-handler attributes (`onerror`, `onclick` and the like), no `javascript:` links, and no `<style>` that restyles the console. Angular's built-in HTML sanitizer is the agreed filter for this: whatever it removes (inline styles, embedded content) is an accepted loss, and whatever it keeps (`class` attributes included) is accepted. Remote images in an email may load; blocking them is a separate ticket.
- Unchanged: the message header shows the sender and the received time as before.

## Pull request description

Agents asked to see customer emails the way the customer wrote them: today the ticket view shows the flattened text part, so lists run together and links lose their text. `TicketMessageComponent` (`src/app/tickets/ticket-message.component.ts`) now renders the email's HTML part, `bodyHtml`, which the API already returns, into the message body instead of the text part. The header is unchanged.

## Context

Angular 21 support console: standalone components, zoneless change detection, `OnPush`, theme tokens from `styles.scss`. `TicketMessage` in `src/app/tickets/ticket.model.ts` (unchanged) is `{ id: string; from: string; receivedAt: string; bodyText: string; bodyHtml: string }`. `bodyHtml` is the HTML part of the email exactly as the inbound mail service received it, at most 1 MB (for a plain-text email the service fills it with the escaped text and `<br>` line breaks). Anyone can email the support address, so its content is written by whoever sent the email. The ticket page (unchanged) renders `<app-ticket-message [message]="m" />` for each message. Agents are signed in to the console with a session cookie that the API accepts on same-origin requests, and an agent can read and answer every customer's tickets. The console sets no Content-Security-Policy yet (a separate ticket). Links in a message open like any other link in the console.

## Diff

`src/app/tickets/ticket-message.component.ts` (modified)

```diff
--- a/src/app/tickets/ticket-message.component.ts
+++ b/src/app/tickets/ticket-message.component.ts
@@ -1,5 +1,6 @@
-import { ChangeDetectionStrategy, Component, input } from '@angular/core';
+import { ChangeDetectionStrategy, Component, computed, inject, input } from '@angular/core';
 import { DatePipe } from '@angular/common';
+import { DomSanitizer } from '@angular/platform-browser';
 import { TicketMessage } from './ticket.model';
 
 @Component({
@@ -12,16 +13,21 @@
         <span class="ticket-message__from">{{ message().from }}</span>
         <time [attr.datetime]="message().receivedAt">{{ message().receivedAt | date: 'medium' }}</time>
       </header>
-      <div class="ticket-message__body">{{ message().bodyText }}</div>
+      <div class="ticket-message__body" [innerHTML]="body()"></div>
     </article>
   `,
   styles: `
     .ticket-message { border: 1px solid var(--border-default); border-radius: 6px; padding: 12px 16px; }
     .ticket-message__header { display: flex; justify-content: space-between; color: var(--fg-muted); }
     .ticket-message__from { font-weight: 600; color: var(--fg-default); }
-    .ticket-message__body { margin-top: 8px; color: var(--fg-default); white-space: pre-wrap; }
+    .ticket-message__body { margin-top: 8px; color: var(--fg-default); overflow-wrap: anywhere; }
   `,
 })
 export class TicketMessageComponent {
+  private readonly sanitizer = inject(DomSanitizer);
+
   readonly message = input.required<TicketMessage>();
+
+  // Keep the customer's formatting as it arrived in the email.
+  readonly body = computed(() => this.sanitizer.bypassSecurityTrustHtml(this.message().bodyHtml));
 }
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [Show customer emails with their formatting in the ticket view](https://app.diffdojo.com/challenges/angular-ticket-email-body?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
