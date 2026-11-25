import React from 'react';

function priorityLabel(priority) {
  return priority === 'high' ? 'High priority' : 'Normal';
}

export default function TicketCard({ ticket }) {
  return (
    <article className="ticket-card">
      <header>
        <h3>{ticket.subject}</h3>
        <span className="priority">{priorityLabel(ticket.priority)}</span>
      </header>
      <footer>#{ticket.id} · opened {ticket.openedAt}</footer>
    </article>
  );
}
