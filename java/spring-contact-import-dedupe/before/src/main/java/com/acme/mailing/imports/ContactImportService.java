package com.acme.mailing.imports;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

@Service
public class ContactImportService {

    private static final Logger log = LoggerFactory.getLogger(ContactImportService.class);

    private final ContactWriter writer;

    public ContactImportService(ContactWriter writer) {
        this.writer = writer;
    }

    public ImportResult importEmails(long listId, List<String> emails) {
        List<String> normalized = new ArrayList<>(emails.size());
        for (String email : emails) {
            normalized.add(normalize(email));
        }
        writer.insertAll(listId, normalized);
        log.info("Imported {} contacts into list {}", normalized.size(), listId);
        return new ImportResult(normalized.size());
    }

    static String normalize(String email) {
        return email.strip().toLowerCase(Locale.ROOT);
    }
}
