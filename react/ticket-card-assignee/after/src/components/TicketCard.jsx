import React from 'react';

function priorityLabel(priority) {
  return priority === 'high' ? 'High priority' : 'Normal';
}

export default function TicketCard({ ticket }) {
  const assigneeName = ticket.assignee.name || 'Unassigned';
  return (
    <article className="ticket-card">
      <header>
        <h3>{ticket.subject}</h3>
        <span className="priority">{priorityLabel(ticket.priority)}</span>
      </header>
      <p className="assignee">Assigned to: {assigneeName}</p>
      <footer>#{ticket.id} · opened {ticket.openedAt}</footer>
    </article>
  );
}
