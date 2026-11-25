import { ChangeDetectionStrategy, Component, computed, inject, input } from '@angular/core';
import { DatePipe } from '@angular/common';
import { DomSanitizer } from '@angular/platform-browser';
import { TicketMessage } from './ticket.model';

@Component({
  selector: 'app-ticket-message',
  imports: [DatePipe],
  changeDetection: ChangeDetectionStrategy.OnPush,
  template: `
    <article class="ticket-message">
      <header class="ticket-message__header">
        <span class="ticket-message__from">{{ message().from }}</span>
        <time [attr.datetime]="message().receivedAt">{{ message().receivedAt | date: 'medium' }}</time>
      </header>
      <div class="ticket-message__body" [innerHTML]="body()"></div>
    </article>
  `,
  styles: `
    .ticket-message { border: 1px solid var(--border-default); border-radius: 6px; padding: 12px 16px; }
    .ticket-message__header { display: flex; justify-content: space-between; color: var(--fg-muted); }
    .ticket-message__from { font-weight: 600; color: var(--fg-default); }
    .ticket-message__body { margin-top: 8px; color: var(--fg-default); overflow-wrap: anywhere; }
  `,
})
export class TicketMessageComponent {
  private readonly sanitizer = inject(DomSanitizer);

  readonly message = input.required<TicketMessage>();

  // Keep the customer's formatting as it arrived in the email.
  readonly body = computed(() => this.sanitizer.bypassSecurityTrustHtml(this.message().bodyHtml));
}
