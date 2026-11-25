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
        List<String> unique = new ArrayList<>(emails.size());
        int duplicates = 0;
        for (String email : emails) {
            String normalized = normalize(email);
            if (unique.contains(normalized)) {
                duplicates++;
            } else {
                unique.add(normalized);
            }
        }
        writer.insertAll(listId, unique);
        log.info("Imported {} contacts into list {}, skipped {} duplicate rows", unique.size(), listId, duplicates);
        return new ImportResult(unique.size(), duplicates);
    }

    static String normalize(String email) {
        return email.strip().toLowerCase(Locale.ROOT);
    }
}
